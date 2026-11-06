// Default OFF. Only timing reads/accumulations are omitted; physical queries retained.
// Incompatible optimization addons must not be combined in the controlled comparison.
modded class DGBNativeGroundBody
{
    bool NFCTPolicyRead;bool NFCTEnabled;
    bool NFCTOn()
    {
        if(!NFCTPolicyRead)
        {
            NFCTEnabled=FileExist("$profile:nf-contact-timers-off.flag");NFCTPolicyRead=true;
            if(NFCTEnabled)Print("NF_CONTACT_TIMING_UNAVAILABLE id="+Id+" physics_queries_unchanged=true");
        }
        return NFCTEnabled;
    }
    override bool SolveContacts(vector anchor,float heading,out float proposedLift,out array<vector> points,float minimumLift=0)
    {
        if(!NFCTOn() || !Director || Director.SupportSweepFast || Director.SupportSweepShadow)
            return super.SolveContacts(anchor,heading,proposedLift,points,minimumLift);

        ContactSolves++;
        bool accepted=NFCTInternal(anchor,heading,proposedLift,points,minimumLift);
        ProposedLift=proposedLift;MaximumProposedLift=Math.Max(MaximumProposedLift,proposedLift);
        return accepted;
    }
    bool NFCTInternal(vector anchor,float heading,out float proposedLift,out array<vector> points,float minimumLift=0)
    {
        proposedLift=0;points=new array<vector>;
        ref array<vector> raw={};

        for(int j=0;j<JointIds.Count();j++)raw.Insert(anchor+Rotate(Leader.GetBonePositionMS(JointIds[j])+IdleOffset,heading));

        array<vector> supportOffsets={"0 0 0","0.065 0 0","-0.065 0 0","0 0 0.065","0 0 -0.065"};
        int liftFoot=2;

        for(int foot=2;foot<4;foot++)
        {
            bool referenceFound=false;float referenceY=-100000;
            bool centerTested=false;bool centerFound=false;
            Object centerObject;vector centerPoint,centerNormal;float centerFraction;
            bool fastEligible=false;float supportBound;
            if(Director.SupportSweepFast || Director.SupportSweepShadow)
            {
                centerTested=true;ContactFootRays++;
                centerFound=DayZPhysics.RayCastBullet(raw[foot]+"0 0.3 0",raw[foot]+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,centerObject,centerPoint,centerNormal,centerFraction);
                if(centerFound && centerNormal[1]>=0.65){referenceFound=true;referenceY=centerPoint[1];}

                if(referenceFound)fastEligible=StructureSupportBound(raw[foot],centerObject,centerPoint,centerNormal,centerFraction,supportBound);

            }
            bool useBound=Director.SupportSweepFast && fastEligible && !Director.SupportSweepShadow;
            if(useBound)
            {referenceFound=true;referenceY=supportBound;SupportFastUsed++;SupportFastSkippedRays+=4;}
            else
            foreach(vector offset:supportOffsets)
            {
                if(centerTested && offset==vector.Zero)continue;
                Object hit;vector point,normal;float fraction;
                vector origin=raw[foot]+offset;
                ContactFootRays++;
                if(DayZPhysics.RayCastBullet(origin+"0 0.3 0",origin+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,hit,point,normal,fraction) && normal[1]>=0.65)
                {
                    referenceFound=true;referenceY=Math.Max(referenceY,point[1]);
                }
            }
            if(Director.SupportSweepShadow && fastEligible && referenceFound)
            {
                SupportFastReferenceChecks++;
                float fastError=referenceY - supportBound;
                if(fastError>0.001)SupportFastUnder++;
                SupportFastMaxUnder=Math.Max(SupportFastMaxUnder,fastError);
                SupportFastMaxOver=Math.Max(SupportFastMaxOver, -fastError);
            }
            if(referenceFound)
            {
                float required=referenceY+Radii[foot]+0.03 - raw[foot][1];
                if(required>proposedLift){proposedLift=required;liftFoot=foot;}
            }
            if(Director.SupportSweepShadow)ObserveSupportSweep(raw[foot],referenceFound,referenceY);
        }
        proposedLift=Math.Max(proposedLift,minimumLift);

        if(proposedLift>0.25)
        {
            Reason="NATIVE_FOOT_LIFT_LIMIT";
            vector oldFoot=raw[liftFoot];if(Contacts.Count()==4)oldFoot=Contacts[liftFoot];

            RecordContactFailure("LIFT_LIMIT",liftFoot,anchor,heading,raw[liftFoot],oldFoot,raw[liftFoot]+Vector(0,proposedLift,0),proposedLift,null);

            return false;
        }
        for(int joint=0;joint<raw.Count();joint++)
        {
            vector target=raw[joint]+Vector(0,proposedLift,0);
            vector previous=target;if(Contacts.Count()==4)previous=Contacts[joint];
            Object collision;vector contact,normalHit;float hitFraction;
            JointSweeps++;

            bool worldContact=DayZPhysics.SphereCastBullet(previous,target,Radii[joint],DGBGroundAgent.Walls(),Leader,collision,contact,normalHit,hitFraction);

            if(worldContact)
            {
                // A tangent/start-overlap with a walkable support is not a wall.
                // Recheck support beneath the proposed foot; side faces still block.
                bool supportedFoot=false;
                if(joint>=2 && normalHit[1]>=0.65)
                {
                    Object floorObject;vector floorPoint,floorNormal;float floorFraction;
                    bool hasFloor=DayZPhysics.RayCastBullet(target+"0 0.3 0",target+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,floorObject,floorPoint,floorNormal,floorFraction);
                    if(!hasFloor)
                    {
                        hasFloor=DayZPhysics.RayCastBullet(contact+"0 0.3 0",contact+"0 -0.3 0",DGBGroundAgent.Floors(),Leader,floorObject,floorPoint,floorNormal,floorFraction);
                    }
                    supportedFoot=hasFloor && floorNormal[1]>=0.65 && target[1] - Radii[joint]>=floorPoint[1] - 0.005;
                }
                if(joint>=2 && !supportedFoot && !collision)
                {

                    supportedFoot=WalkableFootSweep(previous,target,Radii[joint],collision);

                }
                if(!supportedFoot)
                {
                    Reason="NATIVE_JOINT_WORLD_"+joint;
                    HitType="terrain";if(collision && collision.GetType()!="")HitType=collision.GetType();
                    HitNormal=normalHit;HitFraction=hitFraction;HitPoint=contact;

                    RecordContactFailure("SWEEP",joint,anchor,heading,raw[joint],previous,target,proposedLift,collision);

                    return false;
                }
            }
            points.Insert(target);
        }
        return true;
    }
}
