# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T235301-contact-input-observer-20261030

Contact-input observer review packet; selected excerpt, not complete raw evidence.
One 300s live run per population; observer only, no movement policy repair.
CPU and root invariants do not establish visual animation smoothness.
Build F139EB3C6114253EF0F3159A05E7F4386643D27A75C2E839DC4E6B6BD65D170C
Source F1 SHA256 dd8a8b3b89a33b9c933f71ad414f6c141eb666dcafddd970119ab85acb523b13
F1 selection: complete observer wrapper/hook plus original solver body; line numbers below.
F1:1: // Diagnostic only. Generated solver is byte-reversible to the accepted base.
F1:2: // Capture existing vectors, never add bone/physics queries or commit movement.
F1:3: modded class DGBNativeGroundBody
F1:4: {
F1:5:     bool FCRead;bool FCEnabled;
F1:6:     int FCCalls;int FCRows;int FCSuppressed;int FCWriteFailures;
F1:7:     float FCNextAt;string FCLastKey;
F1:8:     static int FCGlobalRows;static int FCGlobalSuppressed;
F1:9:     static float FCFrameAt=-1;static int FCFrameRows;
F1:10:     ref array<vector> FCRaw={};ref array<vector> FCPrevious={};
F1:11:     vector FCAnchor;float FCHeading;float FCMinimumLift;vector FCPositionBefore;
F1:12:     bool FCInside;string FCFailureStage;int FCFailureJoint=-1;
F1:13:     float FCNativeBefore;int FCUpdatesBefore;
F1:14:     bool FCOn()
F1:15:     {
F1:16:         if(!FCRead){FCEnabled=FileExist("$profile:fall-contact-inputs.flag");FCRead=true;}
F1:17:         return FCEnabled;
F1:18:     }
F1:19:     bool FCEligible()
F1:20:     {
F1:21:         if(!Director || !Traversal || !Leader)return false;
F1:22:         if(Traversal.Phase=="FALL")return true;
F1:23:         return Traversal.Phase=="MOTION" && Traversal.Active && Traversal.Active.Action=="CLIMB" && Traversal.Clip && Traversal.NativeCurveEnded && Traversal.MotionElapsed>=Traversal.Clip.duration;
F1:24:     }
F1:25:     void FCObserveRaw(vector anchor,float heading,array<vector> raw,float minimumLift)
F1:26:     {
F1:27:         FCAnchor=anchor;FCHeading=heading;FCMinimumLift=minimumLift;
F1:28:         FCRaw.Copy(raw);FCPrevious.Copy(Contacts);
F1:29:     }
F1:30:     string FCPoint(array<vector> values,int index)
F1:31:     {
F1:32:         if(index>=values.Count())return "UNAVAILABLE";
F1:33:         return values[index].ToString();
F1:34:     }
F1:35:     override void RecordContactFailure(string stage,int joint,vector anchor,float heading,vector raw,vector previous,vector target,float proposedLift,Object collision)
F1:36:     {
F1:37:         if(FCInside){FCFailureStage=stage;FCFailureJoint=joint;}
F1:38:         super.RecordContactFailure(stage,joint,anchor,heading,raw,previous,target,proposedLift,collision);
F1:39:     }
F1:40:     void FCObserveResult(bool accepted,float lift,array<vector> points)
F1:41:     {
F1:42:         string key=Traversal.Phase+":"+Reason+":"+accepted;
F1:43:         bool changed=key!=FCLastKey;
F1:44:         if(!changed && Director.Now<FCNextAt){FCSuppressed++;FCGlobalSuppressed++;return;}
F1:45:         if(FCFrameAt!=Director.Now){FCFrameAt=Director.Now;FCFrameRows=0;}
F1:46:         if(FCRows>=128 || FCGlobalRows>=2048 || FCFrameRows>=4){FCSuppressed++;FCGlobalSuppressed++;return;}
F1:47:         FileHandle file=OpenFile("$profile:native-contact-inputs.tsv",FileMode.APPEND);
F1:48:         if(!file){FCWriteFailures++;return;}
F1:49:         if(FCGlobalRows==0)
F1:50:         {
F1:51:             FPrintln(file,"sequence\tseconds\tid\tcall\tphase\trecoveryGeneration\taccepted\treason\trootBefore\tanchor\theading\tminimumLift\treturnedLift\tidleOffset\trawCount\tpreviousCount\toutputCount\traw0\traw1\traw2\traw3\tprevious0\tprevious1\tprevious2\tprevious3\toutput0\toutput1\toutput2\toutput3\tnativeBefore\tnativeAfter\tupdatesBefore\tupdatesAfter\trequest\tnativeRequest\tcallerFall\tcallerDt\tframeFall\tfallVelocity\tpressureActive\tpressureVelocity\tactiveLink\tactiveExit\tmotionContactOffset\tmotionElapsed\tnativeCurveEnded\tstartPlacementEntry\trootUnchanged\tactorRows\tsuppressedSoFar\tglobalSuppressedSoFar\twriteFailuresSoFar\thitType\thitPoint\thitNormal\thitFraction\tfailureStage\tfailureJoint\trootAfter");
F1:52:         }
F1:53:         FCGlobalRows++;FCFrameRows++;FCRows++;FCNextAt=Director.Now+2;FCLastKey=key;
F1:54:         string active="none";vector exit,placement;
F1:55:         if(Traversal.Active){active=Traversal.Active.Id;exit=Traversal.Active.Exit;}
F1:56:         if(Traversal.StartPlacement)placement=Traversal.StartPlacement.Entry;
F1:57:         string row=FCGlobalRows.ToString()+"\t"+(Director.Now-Director.Born)+"\t"+Id+"\t"+FCCalls+"\t"+Traversal.Phase+"\t"+Traversal.CTRecoveryGeneration;
F1:58:         row+="\t"+accepted+"\t"+Reason+"\t"+FCPositionBefore+"\t"+FCAnchor+"\t"+FCHeading+"\t"+FCMinimumLift+"\t"+lift+"\t"+IdleOffset;
F1:59:         row+="\t"+FCRaw.Count()+"\t"+FCPrevious.Count()+"\t"+points.Count();
F1:60:         for(int j=0;j<4;j++)row+="\t"+FCPoint(FCRaw,j);
F1:61:         for(int k=0;k<4;k++)row+="\t"+FCPoint(FCPrevious,k);
F1:62:         for(int n=0;n<4;n++)row+="\t"+FCPoint(points,n);
F1:63:         row+="\t"+FCNativeBefore+"\t"+Leader.NativeSeconds+"\t"+FCUpdatesBefore+"\t"+Leader.AnimUpdates;
F1:64:         row+="\t"+Request+"\t"+Leader.NativeRequest+"\t"+Traversal.PCInsideFall+"\t"+Traversal.PCFallDt+"\t"+FrameFallPhysicsActive();
F1:65:         row+="\t"+Traversal.FallVelocity+"\t"+Traversal.PressureFallActive+"\t"+Traversal.PressureFallVelocity;
F1:66:         row+="\t"+active+"\t"+exit+"\t"+Traversal.MotionContactOffset+"\t"+Traversal.MotionElapsed+"\t"+Traversal.NativeCurveEnded+"\t"+placement;
F1:67:         // This is a solver-input snapshot, before Accept can commit its root.
F1:68:         bool rootUnchanged=Agent.Position==FCPositionBefore;
F1:69:         row+="\t"+rootUnchanged+"\t"+FCRows+"\t"+FCSuppressed+"\t"+FCGlobalSuppressed+"\t"+FCWriteFailures;
F1:70:         row+="\t"+HitType+"\t"+HitPoint+"\t"+HitNormal+"\t"+HitFraction;
F1:71:         row+="\t"+FCFailureStage+"\t"+FCFailureJoint+"\t"+Agent.Position;
F1:72:         FPrintln(file,row);CloseFile(file);
F1:73:     }
F1:74:     override bool SolveContactsInternal(vector anchor,float heading,out float proposedLift,out array<vector> points,float minimumLift=0)
F1:75:     {
F1:76:         if(!FCOn() || !FCEligible())return super.SolveContactsInternal(anchor,heading,proposedLift,points,minimumLift);
F1:77:         FCCalls++;FCNativeBefore=Leader.NativeSeconds;FCUpdatesBefore=Leader.AnimUpdates;
F1:78:         FCPositionBefore=Agent.Position;FCFailureStage="";FCFailureJoint=-1;
F1:79:         bool oldInside=FCInside;FCInside=true;
F1:80:         bool accepted=FCExecuteSolve(anchor,heading,proposedLift,points,minimumLift);
F1:81:         FCInside=oldInside;
F1:82:         FCObserveResult(accepted,proposedLift,points);
F1:83:         return accepted;
F1:84:     }
F1:85:     bool FCExecuteSolve(vector anchor,float heading,out float proposedLift,out array<vector> points,float minimumLift=0)
F1:86:     {
F1:87:         proposedLift=0;points=new array<vector>;
F1:88:         ref array<vector> raw={};
F1:89:         int boneStarted=TickCount(0);
F1:90:         for(int j=0;j<JointIds.Count();j++)raw.Insert(anchor+Rotate(Leader.GetBonePositionMS(JointIds[j])+IdleOffset,heading));
F1:91:         ContactBoneTicks+=TickCount(boneStarted);
F1:92:         FCObserveRaw(anchor,heading,raw,minimumLift);
F1:93:         array<vector> supportOffsets={"0 0 0","0.065 0 0","-0.065 0 0","0 0 0.065","0 0 -0.065"};
F1:94:         int liftFoot=2;
F1:95:         int footStarted=TickCount(0);
F1:96:         float shadowBefore=SupportShadowTicks;
F1:97:         for(int foot=2;foot<4;foot++)
F1:98:         {
F1:99:             bool referenceFound=false;float referenceY=-100000;
F1:100:             bool centerTested=false;bool centerFound=false;
F1:101:             Object centerObject;vector centerPoint,centerNormal;float centerFraction;
F1:102:             bool fastEligible=false;float supportBound;
F1:103:             if(Director.SupportSweepFast || Director.SupportSweepShadow)
F1:104:             {
F1:105:                 centerTested=true;ContactFootRays++;
F1:106:                 centerFound=DayZPhysics.RayCastBullet(raw[foot]+"0 0.3 0",raw[foot]+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,centerObject,centerPoint,centerNormal,centerFraction);
F1:107:                 if(centerFound && centerNormal[1]>=0.65){referenceFound=true;referenceY=centerPoint[1];}
F1:108:                 int fastStarted=TickCount(0);
F1:109:                 if(referenceFound)fastEligible=StructureSupportBound(raw[foot],centerObject,centerPoint,centerNormal,centerFraction,supportBound);
F1:110:                 SupportFastTicks+=TickCount(fastStarted);
F1:111:             }
F1:112:             bool useBound=Director.SupportSweepFast && fastEligible && !Director.SupportSweepShadow;
F1:113:             if(useBound)
F1:114:             {referenceFound=true;referenceY=supportBound;SupportFastUsed++;SupportFastSkippedRays+=4;}
F1:115:             else
F1:116:             foreach(vector offset:supportOffsets)
F1:117:             {
F1:118:                 if(centerTested && offset==vector.Zero)continue;
F1:119:                 Object hit;vector point,normal;float fraction;
F1:120:                 vector origin=raw[foot]+offset;
F1:121:                 ContactFootRays++;
F1:122:                 if(DayZPhysics.RayCastBullet(origin+"0 0.3 0",origin+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,hit,point,normal,fraction) && normal[1]>=0.65)
F1:123:                 {
F1:124:                     referenceFound=true;referenceY=Math.Max(referenceY,point[1]);
F1:125:                 }
F1:126:             }
F1:127:             if(Director.SupportSweepShadow && fastEligible && referenceFound)
F1:128:             {
F1:129:                 SupportFastReferenceChecks++;
F1:130:                 float fastError=referenceY - supportBound;
F1:131:                 if(fastError>0.001)SupportFastUnder++;
F1:132:                 SupportFastMaxUnder=Math.Max(SupportFastMaxUnder,fastError);
F1:133:                 SupportFastMaxOver=Math.Max(SupportFastMaxOver, -fastError);
F1:134:             }
F1:135:             if(referenceFound)
F1:136:             {
F1:137:                 float required=referenceY+Radii[foot]+0.03 - raw[foot][1];
F1:138:                 if(required>proposedLift){proposedLift=required;liftFoot=foot;}
F1:139:             }
F1:140:             if(Director.SupportSweepShadow)ObserveSupportSweep(raw[foot],referenceFound,referenceY);
F1:141:         }
F1:142:         proposedLift=Math.Max(proposedLift,minimumLift);
F1:143:         ContactFootTicks+=TickCount(footStarted) - (SupportShadowTicks - shadowBefore);
F1:144:         if(proposedLift>0.25)
F1:145:         {
F1:146:             Reason="NATIVE_FOOT_LIFT_LIMIT";
F1:147:             vector oldFoot=raw[liftFoot];if(Contacts.Count()==4)oldFoot=Contacts[liftFoot];
F1:148:             int liftDiagnosticStarted=TickCount(0);
F1:149:             RecordContactFailure("LIFT_LIMIT",liftFoot,anchor,heading,raw[liftFoot],oldFoot,raw[liftFoot]+Vector(0,proposedLift,0),proposedLift,null);
F1:150:             ContactDiagnosticTicks+=TickCount(liftDiagnosticStarted);
F1:151:             return false;
F1:152:         }
F1:153:         for(int joint=0;joint<raw.Count();joint++)
F1:154:         {
F1:155:             vector target=raw[joint]+Vector(0,proposedLift,0);
F1:156:             vector previous=target;if(Contacts.Count()==4)previous=Contacts[joint];
F1:157:             Object collision;vector contact,normalHit;float hitFraction;
F1:158:             JointSweeps++;
F1:159:             int worldStarted=TickCount(0);
F1:160:             bool worldContact=DayZPhysics.SphereCastBullet(previous,target,Radii[joint],DGBGroundAgent.Walls(),Leader,collision,contact,normalHit,hitFraction);
F1:161:             ContactWorldTicks+=TickCount(worldStarted);
F1:162:             if(worldContact)
F1:163:             {
F1:164:                 // A tangent/start-overlap with a walkable support is not a wall.
F1:165:                 // Recheck support beneath the proposed foot; side faces still block.
F1:166:                 bool supportedFoot=false;
F1:167:                 if(joint>=2 && normalHit[1]>=0.65)
F1:168:                 {
F1:169:                     Object floorObject;vector floorPoint,floorNormal;float floorFraction;
F1:170:                     bool hasFloor=DayZPhysics.RayCastBullet(target+"0 0.3 0",target+"0 -1.2 0",DGBGroundAgent.Floors(),Leader,floorObject,floorPoint,floorNormal,floorFraction);
F1:171:                     if(!hasFloor)
F1:172:                     {
F1:173:                         hasFloor=DayZPhysics.RayCastBullet(contact+"0 0.3 0",contact+"0 -0.3 0",DGBGroundAgent.Floors(),Leader,floorObject,floorPoint,floorNormal,floorFraction);
F1:174:                     }
F1:175:                     supportedFoot=hasFloor && floorNormal[1]>=0.65 && target[1] - Radii[joint]>=floorPoint[1] - 0.005;
F1:176:                 }
F1:177:                 if(joint>=2 && !supportedFoot && !collision)
F1:178:                 {
F1:179:                     int fallbackStarted=TickCount(0);
F1:180:                     supportedFoot=WalkableFootSweep(previous,target,Radii[joint],collision);
F1:181:                     ContactFallbackTicks+=TickCount(fallbackStarted);
F1:182:                 }
F1:183:                 if(!supportedFoot)
F1:184:                 {
F1:185:                     Reason="NATIVE_JOINT_WORLD_"+joint;
F1:186:                     HitType="terrain";if(collision && collision.GetType()!="")HitType=collision.GetType();
F1:187:                     HitNormal=normalHit;HitFraction=hitFraction;HitPoint=contact;
F1:188:                     int sweepDiagnosticStarted=TickCount(0);
F1:189:                     RecordContactFailure("SWEEP",joint,anchor,heading,raw[joint],previous,target,proposedLift,collision);
F1:190:                     ContactDiagnosticTicks+=TickCount(sweepDiagnosticStarted);
F1:191:                     return false;
F1:192:                 }
F1:193:             }
F1:194:             points.Insert(target);
F1:195:         }
F1:196:         return true;
F1:197:     }
F1:198: }

Representative rejection selection: first up to eight rejected SWEEP rows in source order.
{
  "sourceRunId": "runtime-20261030-083934",
  "buildId": "F139EB3C6114253EF0F3159A05E7F4386643D27A75C2E839DC4E6B6BD65D170C",
  "verificationStatus": "diagnostic_success",
  "rows": 32,
  "actors": 8,
  "phaseCounts": {
    "MOTION": 17,
    "FALL": 15
  },
  "reasonCounts": {
    "ACCEPTED_NO_REASON": 17,
    "FRAME_FALL_PENDING": 12,
    "NATIVE_JOINT_WORLD_2": 1,
    "NATIVE_JOINT_WORLD_0": 2
  },
  "rejectedSweepCounts": {
    "StaticObj_Container_1C:2": 1,
    "StaticObj_Container_1C:0": 2
  },
  "completePreviousContactRows": 32,
  "fallCallerAvailableRows": 15,
  "perActor": {
    "2": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "1": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "0": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "3": {
      "rows": 11,
      "lastCall": 26,
      "suppressedThroughLastRow": 15
    },
    "6": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "5": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "7": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    },
    "4": {
      "rows": 3,
      "lastCall": 3,
      "suppressedThroughLastRow": 0
    }
  },
  "representativeRejectedSweeps": [
    {
      "sequence": "10",
      "seconds": "15.573",
      "id": "3",
      "call": "4",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_2",
      "rootBefore": "<4511.265625, 340.151337, 10236.701172>",
      "anchor": "<4511.270508, 339.909424, 10236.701172>",
      "heading": "-6",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "2",
      "raw0": "<4511.270508, 340.785614, 10236.701172>",
      "raw1": "<4511.236328, 341.307922, 10237.080078>",
      "raw2": "<4511.063965, 340.055511, 10236.634766>",
      "raw3": "<4511.458008, 339.930908, 10236.886719>",
      "previous0": "<4511.265625, 341.064911, 10236.701172>",
      "previous1": "<4511.231445, 341.587891, 10237.079102>",
      "previous2": "<4511.058594, 340.334717, 10236.635742>",
      "previous3": "<4511.454102, 340.209503, 10236.882813>",
      "output0": "<4511.270508, 340.785614, 10236.701172>",
      "output1": "<4511.236328, 341.307922, 10237.080078>",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "12.65",
      "nativeAfter": "12.65",
      "updatesBefore": "253",
      "updatesAfter": "253",
      "request": "6",
      "nativeRequest": "6",
      "callerFall": "true",
      "callerDt": "0.0200005",
      "frameFall": "true",
      "fallVelocity": "-12.0951",
      "pressureActive": "true",
      "pressureVelocity": "<0.249793, 0.000000, 0.001165>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.016113, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "3",
      "suppressedSoFar": "1",
      "globalSuppressedSoFar": "1",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.062012, 340.168427, 10236.709961>",
      "hitNormal": "<-0.010612, 0.568569, -0.822567>",
      "hitFraction": "0.41119",
      "failureStage": "SWEEP",
      "failureJoint": "2",
      "rootAfter": "<4511.265625, 340.151337, 10236.701172>"
    },
    {
      "sequence": "12",
      "seconds": "15.652",
      "id": "3",
      "call": "8",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_0",
      "rootBefore": "<4511.275391, 339.648285, 10236.677734>",
      "anchor": "<4511.280273, 339.390869, 10236.677734>",
      "heading": "-6",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "0",
      "raw0": "<4511.280273, 340.267059, 10236.677734>",
      "raw1": "<4511.246582, 340.787567, 10237.057617>",
      "raw2": "<4511.075195, 339.537262, 10236.608398>",
      "raw3": "<4511.463867, 339.414398, 10236.871094>",
      "previous0": "<4511.275391, 340.524475, 10236.677734>",
      "previous1": "<4511.241699, 341.045319, 10237.057617>",
      "previous2": "<4511.070313, 339.794617, 10236.609375>",
      "previous3": "<4511.459473, 339.671417, 10236.870117>",
      "output0": "UNAVAILABLE",
      "output1": "UNAVAILABLE",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "12.75",
      "nativeAfter": "12.75",
      "updatesBefore": "255",
      "updatesAfter": "255",
      "request": "6",
      "nativeRequest": "6",
      "callerFall": "true",
      "callerDt": "0.0200005",
      "frameFall": "true",
      "fallVelocity": "-12.8701",
      "pressureActive": "true",
      "pressureVelocity": "<0.249793, 0.000000, 0.001165>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.016113, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "5",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "3",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.282227, 340.172546, 10236.709961>",
      "hitNormal": "<-0.018415, 0.986617, -0.162010>",
      "hitFraction": "0.598722",
      "failureStage": "SWEEP",
      "failureJoint": "0",
      "rootAfter": "<4511.275391, 339.648285, 10236.677734>"
    },
    {
      "sequence": "14",
      "seconds": "15.932",
      "id": "3",
      "call": "22",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_0",
      "rootBefore": "<4511.273438, 339.393311, 10236.520508>",
      "anchor": "<4511.276855, 339.166595, 10236.520508>",
      "heading": "-6",
      "minimumLift": "0",
      "returnedLift": "0.0919189",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "0",
      "raw0": "<4511.276855, 340.042786, 10236.520508>",
      "raw1": "<4511.253418, 340.565582, 10236.901367>",
      "raw2": "<4511.083496, 339.324554, 10236.429688>",
      "raw3": "<4511.443359, 339.199890, 10236.748047>",
      "previous0": "<4511.273438, 340.269501, 10236.520508>",
      "previous1": "<4511.249023, 340.792023, 10236.901367>",
      "previous2": "<4511.079102, 339.549805, 10236.431641>",
      "previous3": "<4511.441406, 339.425812, 10236.746094>",
      "output0": "UNAVAILABLE",
      "output1": "UNAVAILABLE",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "12.95",
      "nativeAfter": "12.95",
      "updatesBefore": "259",
      "updatesAfter": "259",
      "request": "6",
      "nativeRequest": "6",
      "callerFall": "true",
      "callerDt": "0.0200005",
      "frameFall": "true",
      "fallVelocity": "-15",
      "pressureActive": "true",
      "pressureVelocity": "<0.249793, 0.000000, 0.001165>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.016113, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "7",
      "suppressedSoFar": "15",
      "globalSuppressedSoFar": "15",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.275391, 340.172424, 10236.709961>",
      "hitNormal": "<-0.005920, 0.317194, -0.948342>",
      "hitFraction": "0.249424",
      "failureStage": "SWEEP",
      "failureJoint": "0",
      "rootAfter": "<4511.273438, 339.393311, 10236.520508>"
    }
  ],
  "errors": [],
  "limitations": [
    "Bounded sampling; suppression/write-failure counters stop at last emitted row.",
    "Printed seconds do not prove exact frame identity; four-per-frame cap is source/parity evidence.",
    "Snapshots precede root acceptance; matching CPU counters do not prove visible smoothness.",
    "Accepted rows may contain stale hit fields; those fields are not interpreted.",
    "No performance improvement or absence of unsampled failures is asserted."
  ],
  "functionalAcceptance": false,
  "visualAcceptance": false,
  "performanceAcceptance": false,
  "rejectedSweepGeometrySelection": [
    {
      "sequence": 10,
      "id": "3",
      "seconds": 15.573,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 2,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.41119,
      "hitNormal": [
        -0.010612,
        0.568569,
        -0.822567
      ],
      "previousToTargetMetres": 0.2792593609048721,
      "previousToTargetY": -0.27920599999998785,
      "proposalRootDistanceMetres": 0.2419622765184719,
      "previousAvailable": true
    },
    {
      "sequence": 12,
      "id": "3",
      "seconds": 15.652,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 0,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.598722,
      "hitNormal": [
        -0.018415,
        0.986617,
        -0.16201
      ],
      "previousToTargetMetres": 0.2574622904038383,
      "previousToTargetY": -0.2574159999999779,
      "proposalRootDistanceMetres": 0.2574622904038383,
      "previousAvailable": true
    },
    {
      "sequence": 14,
      "id": "3",
      "seconds": 15.932,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 0,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.249424,
      "hitNormal": [
        -0.00592,
        0.317194,
        -0.948342
      ],
      "previousToTargetMetres": 0.13483940249131132,
      "previousToTargetY": -0.13479610000001685,
      "proposalRootDistanceMetres": 0.22674174857093257,
      "previousAvailable": true
    }
  ],
  "rejectedSweepGeometryTotalRows": 3,
  "sourceEvidence": {
    "fileId": "runtime-20261030-083934-contact-inputs",
    "sha256": "31bafbfdec3d083eab9882c596fc935f9c717bf11ebb8c64d014e889d85d0812",
    "bytes": 32116
  }
}

Representative rejection selection: first up to eight rejected SWEEP rows in source order.
{
  "sourceRunId": "runtime-20261030-084554",
  "buildId": "F139EB3C6114253EF0F3159A05E7F4386643D27A75C2E839DC4E6B6BD65D170C",
  "verificationStatus": "diagnostic_success",
  "rows": 1159,
  "actors": 64,
  "phaseCounts": {
    "MOTION": 134,
    "FALL": 1025
  },
  "reasonCounts": {
    "ACCEPTED_NO_REASON": 134,
    "LANDING_SPACE_BLOCKED": 22,
    "NATIVE_FOOT_LIFT_LIMIT": 174,
    "FALL_CONTACT_AIR_SEPARATION": 129,
    "FRAME_FALL_PENDING": 272,
    "NATIVE_FALL": 187,
    "NATIVE_JOINT_WORLD_0": 55,
    "NATIVE_JOINT_WORLD_3": 62,
    "FALL_NATIVE_FOOT_LIFT_LIMIT": 3,
    "NATIVE_JOINT_WORLD_2": 88,
    "FALL_AIR_POSE_ESCAPE_TURN": 4,
    "FALL_NATIVE_JOINT_WORLD_2": 1,
    "NATIVE_JOINT_WORLD_1": 28
  },
  "rejectedSweepCounts": {
    "StaticObj_Container_1C:0": 55,
    "StaticObj_Container_1C:3": 62,
    "StaticObj_Container_1C:2": 88,
    "StaticObj_Container_1C:1": 28
  },
  "completePreviousContactRows": 1159,
  "fallCallerAvailableRows": 1025,
  "perActor": {
    "1": {
      "rows": 14,
      "lastCall": 18,
      "suppressedThroughLastRow": 4
    },
    "2": {
      "rows": 12,
      "lastCall": 15,
      "suppressedThroughLastRow": 3
    },
    "0": {
      "rows": 14,
      "lastCall": 22,
      "suppressedThroughLastRow": 8
    },
    "6": {
      "rows": 25,
      "lastCall": 28,
      "suppressedThroughLastRow": 3
    },
    "5": {
      "rows": 11,
      "lastCall": 13,
      "suppressedThroughLastRow": 2
    },
    "10": {
      "rows": 8,
      "lastCall": 13,
      "suppressedThroughLastRow": 5
    },
    "9": {
      "rows": 23,
      "lastCall": 27,
      "suppressedThroughLastRow": 4
    },
    "14": {
      "rows": 34,
      "lastCall": 45,
      "suppressedThroughLastRow": 11
    },
    "8": {
      "rows": 24,
      "lastCall": 32,
      "suppressedThroughLastRow": 8
    },
    "13": {
      "rows": 25,
      "lastCall": 32,
      "suppressedThroughLastRow": 7
    },
    "11": {
      "rows": 29,
      "lastCall": 49,
      "suppressedThroughLastRow": 20
    },
    "18": {
      "rows": 9,
      "lastCall": 16,
      "suppressedThroughLastRow": 7
    },
    "17": {
      "rows": 24,
      "lastCall": 38,
      "suppressedThroughLastRow": 14
    },
    "16": {
      "rows": 19,
      "lastCall": 30,
      "suppressedThroughLastRow": 11
    },
    "15": {
      "rows": 20,
      "lastCall": 39,
      "suppressedThroughLastRow": 19
    },
    "22": {
      "rows": 31,
      "lastCall": 87,
      "suppressedThroughLastRow": 56
    },
    "12": {
      "rows": 37,
      "lastCall": 51,
      "suppressedThroughLastRow": 14
    },
    "21": {
      "rows": 37,
      "lastCall": 66,
      "suppressedThroughLastRow": 29
    },
    "26": {
      "rows": 18,
      "lastCall": 36,
      "suppressedThroughLastRow": 18
    },
    "25": {
      "rows": 19,
      "lastCall": 31,
      "suppressedThroughLastRow": 12
    },
    "29": {
      "rows": 9,
      "lastCall": 14,
      "suppressedThroughLastRow": 5
    },
    "30": {
      "rows": 14,
      "lastCall": 27,
      "suppressedThroughLastRow": 13
    },
    "24": {
      "rows": 24,
      "lastCall": 32,
      "suppressedThroughLastRow": 8
    },
    "33": {
      "rows": 9,
      "lastCall": 18,
      "suppressedThroughLastRow": 9
    },
    "31": {
      "rows": 25,
      "lastCall": 28,
      "suppressedThroughLastRow": 3
    },
    "34": {
      "rows": 13,
      "lastCall": 28,
      "suppressedThroughLastRow": 15
    },
    "37": {
      "rows": 11,
      "lastCall": 26,
      "suppressedThroughLastRow": 15
    },
    "38": {
      "rows": 13,
      "lastCall": 17,
      "suppressedThroughLastRow": 4
    },
    "32": {
      "rows": 40,
      "lastCall": 49,
      "suppressedThroughLastRow": 9
    },
    "28": {
      "rows": 9,
      "lastCall": 15,
      "suppressedThroughLastRow": 6
    },
    "41": {
      "rows": 9,
      "lastCall": 18,
      "suppressedThroughLastRow": 9
    },
    "36": {
      "rows": 30,
      "lastCall": 36,
      "suppressedThroughLastRow": 6
    },
    "42": {
      "rows": 28,
      "lastCall": 34,
      "suppressedThroughLastRow": 6
    },
    "46": {
      "rows": 33,
      "lastCall": 47,
      "suppressedThroughLastRow": 14
    },
    "40": {
      "rows": 28,
      "lastCall": 31,
      "suppressedThroughLastRow": 3
    },
    "44": {
      "rows": 14,
      "lastCall": 20,
      "suppressedThroughLastRow": 6
    },
    "49": {
      "rows": 8,
      "lastCall": 20,
      "suppressedThroughLastRow": 12
    },
    "54": {
      "rows": 4,
      "lastCall": 16,
      "suppressedThroughLastRow": 12
    },
    "50": {
      "rows": 2,
      "lastCall": 2,
      "suppressedThroughLastRow": 0
    },
    "52": {
      "rows": 13,
      "lastCall": 19,
      "suppressedThroughLastRow": 6
    },
    "53": {
      "rows": 19,
      "lastCall": 30,
      "suppressedThroughLastRow": 11
    },
    "57": {
      "rows": 5,
      "lastCall": 21,
      "suppressedThroughLastRow": 16
    },
    "58": {
      "rows": 4,
      "lastCall": 5,
      "suppressedThroughLastRow": 1
    },
    "55": {
      "rows": 4,
      "lastCall": 13,
      "suppressedThroughLastRow": 9
    },
    "56": {
      "rows": 16,
      "lastCall": 26,
      "suppressedThroughLastRow": 10
    },
    "62": {
      "rows": 46,
      "lastCall": 58,
      "suppressedThroughLastRow": 12
    },
    "19": {
      "rows": 12,
      "lastCall": 29,
      "suppressedThroughLastRow": 17
    },
    "61": {
      "rows": 23,
      "lastCall": 29,
      "suppressedThroughLastRow": 6
    },
    "45": {
      "rows": 22,
      "lastCall": 27,
      "suppressedThroughLastRow": 5
    },
    "48": {
      "rows": 8,
      "lastCall": 14,
      "suppressedThroughLastRow": 6
    },
    "35": {
      "rows": 15,
      "lastCall": 19,
      "suppressedThroughLastRow": 4
    },
    "7": {
      "rows": 21,
      "lastCall": 25,
      "suppressedThroughLastRow": 4
    },
    "4": {
      "rows": 24,
      "lastCall": 28,
      "suppressedThroughLastRow": 4
    },
    "27": {
      "rows": 41,
      "lastCall": 80,
      "suppressedThroughLastRow": 39
    },
    "47": {
      "rows": 3,
      "lastCall": 10,
      "suppressedThroughLastRow": 7
    },
    "39": {
      "rows": 8,
      "lastCall": 26,
      "suppressedThroughLastRow": 18
    },
    "63": {
      "rows": 15,
      "lastCall": 20,
      "suppressedThroughLastRow": 5
    },
    "43": {
      "rows": 10,
      "lastCall": 16,
      "suppressedThroughLastRow": 6
    },
    "59": {
      "rows": 4,
      "lastCall": 18,
      "suppressedThroughLastRow": 14
    },
    "20": {
      "rows": 23,
      "lastCall": 28,
      "suppressedThroughLastRow": 5
    },
    "51": {
      "rows": 16,
      "lastCall": 30,
      "suppressedThroughLastRow": 14
    },
    "3": {
      "rows": 7,
      "lastCall": 16,
      "suppressedThroughLastRow": 9
    },
    "23": {
      "rows": 23,
      "lastCall": 32,
      "suppressedThroughLastRow": 9
    },
    "60": {
      "rows": 21,
      "lastCall": 25,
      "suppressedThroughLastRow": 4
    }
  },
  "representativeRejectedSweeps": [
    {
      "sequence": "27",
      "seconds": "18.006",
      "id": "8",
      "call": "13",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_0",
      "rootBefore": "<4508.466309, 339.398102, 10237.564453>",
      "anchor": "<4508.451660, 339.118011, 10237.587891>",
      "heading": "24",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "0",
      "raw0": "<4508.462402, 340.097351, 10237.586914>",
      "raw1": "<4508.573730, 340.756042, 10237.767578>",
      "raw2": "<4508.385742, 339.540710, 10237.854492>",
      "raw3": "<4508.717285, 339.278992, 10237.727539>",
      "previous0": "<4508.473633, 340.377563, 10237.565430>",
      "previous1": "<4508.571777, 341.033630, 10237.758789>",
      "previous2": "<4508.363770, 339.780792, 10237.723633>",
      "previous3": "<4508.781738, 339.621887, 10237.796875>",
      "output0": "UNAVAILABLE",
      "output1": "UNAVAILABLE",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "12.9",
      "nativeAfter": "12.9",
      "updatesBefore": "258",
      "updatesAfter": "258",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-6.08162",
      "pressureActive": "true",
      "pressureVelocity": "<-0.381835, 0.000000, 0.621766>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<-0.084473, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "10",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "3",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4508.645020, 340.125641, 10237.578125>",
      "hitNormal": "<-0.888950, 0.458003, -0.001265>",
      "hitFraction": "0.572341",
      "failureStage": "SWEEP",
      "failureJoint": "0",
      "rootAfter": "<4508.466309, 339.398102, 10237.564453>"
    },
    {
      "sequence": "59",
      "seconds": "23.059",
      "id": "32",
      "call": "10",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_3",
      "rootBefore": "<4508.458008, 339.516785, 10237.421875>",
      "anchor": "<4508.431152, 339.228394, 10237.444336>",
      "heading": "18",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "3",
      "raw0": "<4508.435059, 340.207520, 10237.445313>",
      "raw1": "<4508.499023, 340.858704, 10237.661133>",
      "raw2": "<4508.290527, 339.538696, 10237.517578>",
      "raw3": "<4508.721191, 339.527466, 10237.737305>",
      "previous0": "<4508.458008, 340.392975, 10237.421875>",
      "previous1": "<4508.614746, 340.924255, 10237.769531>",
      "previous2": "<4508.241699, 339.738831, 10237.341797>",
      "previous3": "<4508.666504, 339.556183, 10237.547852>",
      "output0": "<4508.435059, 340.207520, 10237.445313>",
      "output1": "<4508.499023, 340.858704, 10237.661133>",
      "output2": "<4508.290527, 339.538696, 10237.517578>",
      "output3": "UNAVAILABLE",
      "nativeBefore": "16.35",
      "nativeAfter": "16.35",
      "updatesBefore": "327",
      "updatesAfter": "327",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-5.76767",
      "pressureActive": "true",
      "pressureVelocity": "<-0.533829, 0.000000, 0.440615>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<-0.040527, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "7",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "7",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4508.712402, 339.557037, 10237.547852>",
      "hitNormal": "<-0.999826, -0.018661, -0.000001>",
      "hitFraction": "0",
      "failureStage": "SWEEP",
      "failureJoint": "3",
      "rootAfter": "<4508.458008, 339.516785, 10237.421875>"
    },
    {
      "sequence": "69",
      "seconds": "23.687",
      "id": "36",
      "call": "11",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_3",
      "rootBefore": "<4508.437500, 339.561035, 10237.105469>",
      "anchor": "<4508.410645, 339.258423, 10237.088867>",
      "heading": "12",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "3",
      "raw0": "<4508.414063, 340.237549, 10237.089844>",
      "raw1": "<4508.455566, 340.888733, 10237.311523>",
      "raw2": "<4508.263184, 339.568726, 10237.147461>",
      "raw3": "<4508.668457, 339.557495, 10237.411133>",
      "previous0": "<4508.437500, 340.437225, 10237.105469>",
      "previous1": "<4508.560059, 340.968872, 10237.466797>",
      "previous2": "<4508.232910, 339.784760, 10236.999023>",
      "previous3": "<4508.629395, 339.604797, 10237.270508>",
      "output0": "<4508.414063, 340.237549, 10237.089844>",
      "output1": "<4508.455566, 340.888733, 10237.311523>",
      "output2": "<4508.263184, 339.568726, 10237.147461>",
      "output3": "UNAVAILABLE",
      "nativeBefore": "16.65",
      "nativeAfter": "16.65",
      "updatesBefore": "333",
      "updatesAfter": "333",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-6.0522",
      "pressureActive": "true",
      "pressureVelocity": "<-0.535435, 0.000000, -0.323995>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<-0.107910, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "8",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "10",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4508.711914, 339.606323, 10237.270508>",
      "hitNormal": "<-0.999826, -0.018661, 0.000001>",
      "hitFraction": "0",
      "failureStage": "SWEEP",
      "failureJoint": "3",
      "rootAfter": "<4508.437500, 339.561035, 10237.105469>"
    },
    {
      "sequence": "128",
      "seconds": "49.859",
      "id": "60",
      "call": "4",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_3",
      "rootBefore": "<4511.355469, 340.050079, 10238.050781>",
      "anchor": "<4511.364746, 340.021271, 10238.046875>",
      "heading": "-1.83054",
      "minimumLift": "0",
      "returnedLift": "0.213623",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "3",
      "raw0": "<4511.364746, 340.848877, 10238.046875>",
      "raw1": "<4511.369141, 341.178772, 10238.662109>",
      "raw2": "<4511.121094, 340.117950, 10238.453125>",
      "raw3": "<4511.646973, 340.115387, 10237.741211>",
      "previous0": "<4511.355469, 341.062317, 10238.050781>",
      "previous1": "<4511.359863, 341.392212, 10238.666016>",
      "previous2": "<4511.111816, 340.331390, 10238.457031>",
      "previous3": "<4511.637695, 340.328827, 10237.745117>",
      "output0": "<4511.364746, 341.062500, 10238.046875>",
      "output1": "<4511.369141, 341.392395, 10238.662109>",
      "output2": "<4511.121094, 340.331573, 10238.453125>",
      "output3": "UNAVAILABLE",
      "nativeBefore": "33.8998",
      "nativeAfter": "33.8998",
      "updatesBefore": "678",
      "updatesAfter": "678",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.0149918",
      "frameFall": "true",
      "fallVelocity": "-1.92205",
      "pressureActive": "true",
      "pressureVelocity": "<0.626243, 0.000000, -0.265109>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.200195, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "3",
      "suppressedSoFar": "1",
      "globalSuppressedSoFar": "13",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.733398, 340.330261, 10237.742188>",
      "hitNormal": "<-0.999892, -0.014669, 0.000002>",
      "hitFraction": "0.615835",
      "failureStage": "SWEEP",
      "failureJoint": "3",
      "rootAfter": "<4511.355469, 340.050079, 10238.050781>"
    },
    {
      "sequence": "137",
      "seconds": "50.233",
      "id": "60",
      "call": "14",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_2",
      "rootBefore": "<4511.431641, 339.693878, 10238.008789>",
      "anchor": "<4511.462891, 339.425140, 10237.995117>",
      "heading": "-1.83054",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "2",
      "raw0": "<4511.478516, 340.403656, 10237.997070>",
      "raw1": "<4511.535156, 341.058868, 10238.205078>",
      "raw2": "<4511.273438, 339.808441, 10238.350586>",
      "raw3": "<4511.615723, 339.568146, 10238.003906>",
      "previous0": "<4511.445313, 340.887543, 10238.012695>",
      "previous1": "<4511.485352, 341.545624, 10238.219727>",
      "previous2": "<4511.246582, 340.331116, 10238.315430>",
      "previous3": "<4511.599609, 340.043243, 10238.125977>",
      "output0": "<4511.478516, 340.403656, 10237.997070>",
      "output1": "<4511.535156, 341.058868, 10238.205078>",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "34.1498",
      "nativeAfter": "34.1498",
      "updatesBefore": "683",
      "updatesAfter": "683",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-5.37504",
      "pressureActive": "true",
      "pressureVelocity": "<0.626243, 0.000000, -0.265109>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.200195, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "11",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "15",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.230957, 340.211151, 10238.317383>",
      "hitNormal": "<0.191656, 0.981459, -0.002608>",
      "hitFraction": "0.0603023",
      "failureStage": "SWEEP",
      "failureJoint": "2",
      "rootAfter": "<4511.431641, 339.693878, 10238.008789>"
    },
    {
      "sequence": "139",
      "seconds": "50.298",
      "id": "60",
      "call": "16",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_2",
      "rootBefore": "<4511.431641, 339.693878, 10238.008789>",
      "anchor": "<4511.462891, 339.389801, 10237.995117>",
      "heading": "-1.83054",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "2",
      "raw0": "<4511.478516, 340.367981, 10237.995117>",
      "raw1": "<4511.532715, 341.021759, 10238.206055>",
      "raw2": "<4511.277832, 339.708557, 10238.350586>",
      "raw3": "<4511.601563, 339.570831, 10237.940430>",
      "previous0": "<4511.445313, 340.887543, 10238.012695>",
      "previous1": "<4511.485352, 341.545624, 10238.219727>",
      "previous2": "<4511.246582, 340.331116, 10238.315430>",
      "previous3": "<4511.599609, 340.043243, 10238.125977>",
      "output0": "<4511.478516, 340.367981, 10237.995117>",
      "output1": "<4511.532715, 341.021759, 10238.206055>",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "34.1998",
      "nativeAfter": "34.1998",
      "updatesBefore": "684",
      "updatesAfter": "684",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-6.08132",
      "pressureActive": "true",
      "pressureVelocity": "<0.626243, 0.000000, -0.265109>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.200195, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "13",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "15",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.230957, 340.211121, 10238.317383>",
      "hitNormal": "<0.195222, 0.980756, -0.002605>",
      "hitFraction": "0.0506199",
      "failureStage": "SWEEP",
      "failureJoint": "2",
      "rootAfter": "<4511.431641, 339.693878, 10238.008789>"
    },
    {
      "sequence": "144",
      "seconds": "50.437",
      "id": "60",
      "call": "19",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_0",
      "rootBefore": "<4511.462891, 339.357941, 10237.986328>",
      "anchor": "<4511.468750, 339.289642, 10237.984375>",
      "heading": "4.16946",
      "minimumLift": "0",
      "returnedLift": "0",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "0",
      "raw0": "<4511.477539, 340.268036, 10237.982422>",
      "raw1": "<4511.514648, 340.923492, 10238.197266>",
      "raw2": "<4511.323242, 339.495056, 10238.238281>",
      "raw3": "<4511.594727, 339.599030, 10237.987305>",
      "previous0": "<4511.471680, 340.336334, 10237.984375>",
      "previous1": "<4511.508789, 340.991791, 10238.199219>",
      "previous2": "<4511.317383, 339.563354, 10238.240234>",
      "previous3": "<4511.588867, 339.667328, 10237.989258>",
      "output0": "UNAVAILABLE",
      "output1": "UNAVAILABLE",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "34.2998",
      "nativeAfter": "34.2998",
      "updatesBefore": "686",
      "updatesAfter": "686",
      "request": "8",
      "nativeRequest": "8",
      "callerFall": "true",
      "callerDt": "0.00899506",
      "frameFall": "true",
      "fallVelocity": "-7.59199",
      "pressureActive": "true",
      "pressureVelocity": "<0.626243, 0.000000, -0.265109>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.200195, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "16",
      "suppressedSoFar": "3",
      "globalSuppressedSoFar": "15",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.305664, 340.176361, 10237.982422>",
      "hitNormal": "<0.855776, 0.517346, -0.001335>",
      "hitFraction": "0.827674",
      "failureStage": "SWEEP",
      "failureJoint": "0",
      "rootAfter": "<4511.462891, 339.357941, 10237.986328>"
    },
    {
      "sequence": "145",
      "seconds": "50.588",
      "id": "14",
      "call": "2",
      "phase": "FALL",
      "recoveryGeneration": "1",
      "accepted": "false",
      "reason": "NATIVE_JOINT_WORLD_2",
      "rootBefore": "<4511.336914, 340.183960, 10240.844727>",
      "anchor": "<4511.369141, 340.109436, 10240.862305>",
      "heading": "166.584",
      "minimumLift": "0",
      "returnedLift": "0.123779",
      "idleOffset": "<0.000000, 0.000000, 0.000000>",
      "rawCount": "4",
      "previousCount": "4",
      "outputCount": "2",
      "raw0": "<4511.369141, 340.925507, 10240.862305>",
      "raw1": "<4511.404785, 341.251343, 10240.250000>",
      "raw2": "<4511.645020, 340.205627, 10240.526367>",
      "raw3": "<4510.985352, 340.212341, 10241.112305>",
      "previous0": "<4511.336914, 341.060547, 10240.844727>",
      "previous1": "<4511.411621, 341.392578, 10240.237305>",
      "previous2": "<4511.629395, 340.335999, 10240.500977>",
      "previous3": "<4510.971680, 340.335815, 10241.090820>",
      "output0": "<4511.369141, 341.049286, 10240.862305>",
      "output1": "<4511.404785, 341.375122, 10240.250000>",
      "output2": "UNAVAILABLE",
      "output3": "UNAVAILABLE",
      "nativeBefore": "34.3998",
      "nativeAfter": "34.3998",
      "updatesBefore": "688",
      "updatesAfter": "688",
      "request": "8",
      "nativeRequest": "0",
      "callerFall": "true",
      "callerDt": "0.05",
      "frameFall": "true",
      "fallVelocity": "-1.4905",
      "pressureActive": "true",
      "pressureVelocity": "<0.644179, 0.000000, 0.344240>",
      "activeLink": "none",
      "activeExit": "<0.000000, 0.000000, 0.000000>",
      "motionContactOffset": "<0.114746, 0.000000, 0.000000>",
      "motionElapsed": "0",
      "nativeCurveEnded": "true",
      "startPlacementEntry": "<0.000000, 0.000000, 0.000000>",
      "rootUnchanged": "true",
      "actorRows": "2",
      "suppressedSoFar": "0",
      "globalSuppressedSoFar": "15",
      "writeFailuresSoFar": "0",
      "hitType": "StaticObj_Container_1C",
      "hitPoint": "<4511.733398, 340.331421, 10240.523438>",
      "hitNormal": "<-0.999892, -0.014667, 0.000000>",
      "hitFraction": "0.895812",
      "failureStage": "SWEEP",
      "failureJoint": "2",
      "rootAfter": "<4511.336914, 340.183960, 10240.844727>"
    }
  ],
  "errors": [],
  "limitations": [
    "Bounded sampling; suppression/write-failure counters stop at last emitted row.",
    "Printed seconds do not prove exact frame identity; four-per-frame cap is source/parity evidence.",
    "Snapshots precede root acceptance; matching CPU counters do not prove visible smoothness.",
    "Accepted rows may contain stale hit fields; those fields are not interpreted.",
    "No performance improvement or absence of unsampled failures is asserted."
  ],
  "functionalAcceptance": false,
  "visualAcceptance": false,
  "performanceAcceptance": false,
  "rejectedSweepGeometrySelection": [
    {
      "sequence": 27,
      "id": "8",
      "seconds": 18.006,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 0,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.572341,
      "hitNormal": [
        -0.88895,
        0.458003,
        -0.001265
      ],
      "previousToTargetMetres": 0.28125871108455913,
      "previousToTargetY": -0.2802120000000059,
      "proposalRootDistanceMetres": 0.2814514191222438,
      "previousAvailable": true
    },
    {
      "sequence": 59,
      "id": "32",
      "seconds": 23.059,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 3,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.0,
      "hitNormal": [
        -0.999826,
        -0.018661,
        -1e-06
      ],
      "previousToTargetMetres": 0.1992680939521138,
      "previousToTargetY": -0.028716999999971904,
      "proposalRootDistanceMetres": 0.2905083650052464,
      "previousAvailable": true
    },
    {
      "sequence": 69,
      "id": "36",
      "seconds": 23.687,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 3,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.0,
      "hitNormal": [
        -0.999826,
        -0.018661,
        1e-06
      ],
      "previousToTargetMetres": 0.15342330224899658,
      "previousToTargetY": -0.047302000000001954,
      "proposalRootDistanceMetres": 0.30425456442426607,
      "previousAvailable": true
    },
    {
      "sequence": 128,
      "id": "60",
      "seconds": 49.859,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 3,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.615835,
      "hitNormal": [
        -0.999892,
        -0.014669,
        2e-06
      ],
      "previousToTargetMetres": 0.0100683468847076,
      "previousToTargetY": 0.00018299999999271677,
      "proposalRootDistanceMetres": 0.03051590452533961,
      "previousAvailable": true
    },
    {
      "sequence": 137,
      "id": "60",
      "seconds": 50.233,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 2,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.0603023,
      "hitNormal": [
        0.191656,
        0.981459,
        -0.002608
      ],
      "previousToTargetMetres": 0.5245439397200277,
      "previousToTargetY": -0.5226749999999925,
      "proposalRootDistanceMetres": 0.27089407289191125,
      "previousAvailable": true
    },
    {
      "sequence": 139,
      "id": "60",
      "seconds": 50.298,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 2,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.0506199,
      "hitNormal": [
        0.195222,
        0.980756,
        -0.002605
      ],
      "previousToTargetMetres": 0.6243334167870752,
      "previousToTargetY": -0.6225590000000238,
      "proposalRootDistanceMetres": 0.3059841630100818,
      "previousAvailable": true
    },
    {
      "sequence": 144,
      "id": "60",
      "seconds": 50.437,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 0,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.827674,
      "hitNormal": [
        0.855776,
        0.517346,
        -0.001335
      ],
      "previousToTargetMetres": 0.06857666435469323,
      "previousToTargetY": -0.06829800000002706,
      "proposalRootDistanceMetres": 0.06857766029107064,
      "previousAvailable": true
    },
    {
      "sequence": 145,
      "id": "14",
      "seconds": 50.588,
      "phase": "FALL",
      "recoveryGeneration": 1,
      "joint": 2,
      "hitType": "StaticObj_Container_1C",
      "hitFraction": 0.895812,
      "hitNormal": [
        -0.999892,
        -0.014667,
        0.0
      ],
      "previousToTargetMetres": 0.030532939164704793,
      "previousToTargetY": -0.006593000000009397,
      "proposalRootDistanceMetres": 0.08307461819960553,
      "previousAvailable": true
    }
  ],
  "rejectedSweepGeometryTotalRows": 233,
  "sourceEvidence": {
    "fileId": "runtime-20261030-084554-contact-inputs",
    "sha256": "a711125de7cb888c4067e87a7f3f38a4af8575c2ad0fe994e23ad5d7e9c860e7",
    "bytes": 1129104
  }
}

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
