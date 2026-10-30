# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261030T001442-actor60-direction-diagnosis-20261030

Actor60 direction diagnosis;selected frozen inputs and functions,not full gameplay evidence.
Source run runtime-20261030-084554. No new client run in this bundle.
CPU pose frozen,other agents absent. Direct rays and checked diagnostic movement only.
Primitive and rolling repeats byte-identical. Failed initial and syntax trials preserved separately.

F1 DGBNativeTraversal.c SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872
F1:867: bool PlanPressureEscape(float dt)
F1:868:     {
F1:869:         PressureEscapePlans++;PressureEscapeValid=false;NextPressureEscapePlan=Now()+0.75;
F1:870:         array<vector> directions={"0 0 1","0 0 -1","1 0 0","-1 0 0","0.707107 0 0.707107","-0.707107 0 0.707107","0.707107 0 -0.707107","-0.707107 0 -0.707107"};
F1:871:         array<bool> blockedFirstSteps={false,false,false,false,false,false,false,false};
F1:872:         ref array<string> outcomes={"TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0","TARGET_NONE\t0 0 0"};
F1:873:         bool tracePlan=EscapeNoExitAt>=0 && Now() - EscapeNoExitAt>=2 && Now()>=NextEscapePlanTrace;
F1:874:         string savedLanding=LandingDiagnostic;
F1:875:         for(float distance=0.5;distance<=4;distance+=0.5)
F1:876:         {
F1:877:             for(int directionIndex=0;directionIndex<directions.Count();directionIndex++)
F1:878:             {
F1:879:                 // Only within this immutable plan call: farther targets along
F1:880:                 // one direction use exactly the same first physical step.
F1:881:                 if(blockedFirstSteps[directionIndex]){EscapeDuplicateDirectionsAvoided++;continue;}
F1:882:                 vector direction=directions[directionIndex];
F1:883:                 vector target=Body.Agent.Position+direction*distance;vector supported;
F1:884:                 if(Phase=="FALL")target=DGBTraversalFixture.Ground(target);
F1:885:                 else
F1:886:                 {
F1:887:                     vector candidateSupport;
F1:888:                     if(!DGBGroundAgent.Floor(Body.Leader,target,candidateSupport)){if(tracePlan)outcomes[directionIndex]="TARGET_NO_FLOOR\t"+target;continue;}
F1:889:                     if(Fixture().Area(candidateSupport)!=Fixture().Area(Body.Agent.Position)){if(tracePlan)outcomes[directionIndex]="TARGET_AREA_CHANGED\t"+target;continue;}
F1:890:                     if(Math.AbsFloat(candidateSupport[1] - Body.Agent.Position[1])>0.08){if(tracePlan)outcomes[directionIndex]="TARGET_HEIGHT_CHANGED\t"+target;continue;}
F1:891:                     target=candidateSupport;
F1:892:                 }
F1:893:                 if(!LandingSpace(target,supported)){if(tracePlan)outcomes[directionIndex]="TARGET_"+LandingDiagnostic+"\t"+target;continue;}
F1:894:                 float candidateLift;ref array<vector> points={};
F1:895:                 if(!PressureEscapeStepClear(Body.Agent.Position+direction*Math.Min(0.04,dt*0.6),candidateLift,points))
F1:896:                 {
F1:897:                     PressureEscapeLastStepReject=PressureEscapeDiagnostic;blockedFirstSteps[directionIndex]=true;
F1:898:                     if(tracePlan)outcomes[directionIndex]=EscapeDirectionFailure(supported);
F1:899:                     continue;
F1:900:                 }
F1:901:                 EscapeNoExitAt=-1;
F1:902:                 PressureEscapeGoal=supported;PressureEscapeValid=true;LandingDiagnostic=savedLanding;return true;
F1:903:             }
F1:904:         }
F1:905:         if(EscapeNoExitAt<0)EscapeNoExitAt=Now();
F1:906:         if(tracePlan)TraceEscapePlan(outcomes,dt);
F1:907:         LandingDiagnostic=savedLanding;PressureEscapeDiagnostic="NO_VALID_LOCAL_EXIT";return false;
F1:908:     }
F1:934: bool PressureSlide(float dt)
F1:935:     {
F1:936:         if(!PressureFallActive && !(Phase=="LAND" && PressureLandingSeparation) && !(Phase=="FOLLOW" && PoseClearanceActive))return false;
F1:937:         if(SeparationEpisodeDistance>=SeparationDistanceBudget){PressureEscapeDiagnostic="SEPARATION_DISTANCE_LIMIT";return false;}
F1:938:         if(!PressureEscapeValid && Now()<NextPressureEscapePlan)return false;
F1:939:         if(!PressureEscapeValid && !PlanPressureEscape(dt))return false;
F1:940:         vector start=Body.Agent.Position;vector direction=PressureEscapeGoal - start;direction[1]=0;
F1:941:         float remaining=direction.Length();
F1:942:         if(remaining<0.04){PressureEscapeValid=false;NextPressureEscapePlan=0;return false;}
F1:943:         direction.Normalize();float step=Math.Min(SeparationDistanceBudget - SeparationEpisodeDistance,Math.Min(remaining,Math.Min(0.04,dt*0.6)));
F1:944:         vector next=start+direction*step;
F1:945:         if(Phase!="FALL")
F1:946:         {
F1:947:             vector support;
F1:948:             bool sameSupport=DGBGroundAgent.Floor(Body.Leader,next,support) && Fixture().Area(support)==Fixture().Area(start) && Math.AbsFloat(support[1] - start[1])<=0.08;
F1:949:             if(!sameSupport && Fixture().Area(next)==Fixture().Area(start) && (Fixture().Area(start)=="low" || Fixture().Area(start)=="high"))
F1:950:             {
F1:951:                 // The accepted root may straddle a rim. Preserve its height
F1:952:                 // only when an actual animated foot still hits that roof and
F1:953:                 // this step approaches the already validated interior target.
F1:954:                 for(int supportFoot=2;supportFoot<Body.JointIds.Count();supportFoot++)
F1:955:                 {
F1:956:                     vector supportProbe=Body.JointWorldPoint(supportFoot,next,Body.Agent.Heading,Body.Lift);
F1:957:                     Object supportObject;vector footSupport,footNormal;float footFraction;
F1:958:                     if(DayZPhysics.RayCastBullet(supportProbe+"0 0.3 0",supportProbe+"0 -1.2 0",DGBGroundAgent.Floors(),Body.Leader,supportObject,footSupport,footNormal,footFraction) && footNormal[1]>=0.65 && Fixture().Area(footSupport)==Fixture().Area(start) && Math.AbsFloat(footSupport[1] - start[1])<=0.08 && supportProbe[1] - Body.Radii[supportFoot]>=footSupport[1] - 0.005 && supportProbe[1] - Body.Radii[supportFoot]<=footSupport[1]+0.06)
F1:959:                     {sameSupport=true;support=next;PartialSupportChecks++;break;}
F1:960:                 }
F1:961:             }
F1:962:             if(!sameSupport)
F1:963:             {PressureEscapeDiagnostic="SUPPORTED_SURFACE_CHANGED";PressureEscapeValid=false;return false;}
F1:964:             next=support;
F1:965:         }
F1:966:         float lift;ref array<vector> contacts={};
F1:967:         if(!PressureEscapeStepClear(next,lift,contacts))
F1:968:         {PressureEscapeRejected++;PressureEscapeLastStepReject=PressureEscapeDiagnostic;PressureEscapeValid=false;return false;}
F1:969:         float travel=vector.Distance(start,next);Body.Agent.Position=next;Body.Agent.Moved+=travel;
F1:970:         SeparationEpisodeDistance+=travel;PressureEscapeSteps++;Body.Lift=lift;Body.Contacts.Copy(contacts);
F1:971:         if(Phase=="FALL"){Body.PressureAirDistance+=travel;Body.PressureAirSteps++;}
F1:972:         if(Phase=="LAND")LandingSeparationSteps++;
F1:973:         if(Phase=="FOLLOW")PoseClearanceSteps++;
F1:974:         Body.Reason="CONTACT_AIR_SEPARATION";return true;
F1:975:     }
F1:976: bool SupportedEscapeTurn(float dt)
F1:977:     {
F1:978:         // Supported recovery still requires a real landing footprint.
F1:979:         bool supportedMode=Phase=="LAND" && PressureLandingSeparation;
F1:980:         if(Phase=="FOLLOW" && PoseClearanceActive && !EntryAligning && !Active)supportedMode=true;
F1:981:         if(!supportedMode || Now()<NextSupportedEscapeTurn)return false;
F1:982:         vector supported;
F1:983:         if(!LandingSpace(Body.Agent.Position,supported,false) || vector.Distance(supported,Body.Agent.Position)>0.03)return false;
F1:984:         return SweepEscapeTurn(dt,false);
F1:985:     }

F2 actor60_refined_escape_traversal.c SHA256 f49d3b5f5f0a919213a65cf9cbff78a993e10d07bffee914e9f892fe582bf50d
F2:1: // Isolated frozen-pose experiment. This class is not in a deployed PBO.
F2:2: class DGActor60RefinedTraversal: DGBNativeTraversal
F2:3: {
F2:4:     bool RefineEnabled;int RefinementPlans;int RefinementTargets;int RefinementSweeps;int RefinementAccepted;
F2:5:     void DGActor60RefinedTraversal(DGBNativeGroundBody body){Body=body;PhaseAt=body.Director.Now;}
F2:6:     override bool PlanPressureEscape(float dt)
F2:7:     {
F2:8:         bool planned=super.PlanPressureEscape(dt);
F2:9:         if(planned || !RefineEnabled || Phase!="FOLLOW" || Active || EntryAligning)return planned;
F2:10:         RefinementPlans++;
F2:11:         array<bool> blocked={false,false,false,false,false,false,false,false};
F2:12:         string savedLanding=LandingDiagnostic;
F2:13:         for(float distance=0.5;distance<=4;distance+=0.5)
F2:14:         for(int index=0;index<8;index++)
F2:15:         {
F2:16:             if(blocked[index])continue;
F2:17:             float angle=(22.5+45*index)*Math.DEG2RAD;
F2:18:             vector direction=Vector(Math.Sin(angle),0,Math.Cos(angle));
F2:19:             vector candidate=Body.Agent.Position+direction*distance;
F2:20:             vector floor,supported;RefinementTargets++;
F2:21:             if(!DGBGroundAgent.Floor(Body.Leader,candidate,floor))continue;
F2:22:             if(Fixture().Area(floor)!=Fixture().Area(Body.Agent.Position))continue;
F2:23:             if(Math.AbsFloat(floor[1] - Body.Agent.Position[1])>0.08)continue;
F2:24:             if(!LandingSpace(floor,supported))continue;
F2:25:             float lift;ref array<vector> points={};RefinementSweeps++;
F2:26:             if(!PressureEscapeStepClear(Body.Agent.Position+direction*Math.Min(0.04,dt*0.6),lift,points))
F2:27:             {blocked[index]=true;continue;}
F2:28:             EscapeNoExitAt=-1;PressureEscapeGoal=supported;PressureEscapeValid=true;
F2:29:             LandingDiagnostic=savedLanding;RefinementAccepted++;return true;
F2:30:         }
F2:31:         LandingDiagnostic=savedLanding;return false;
F2:32:     }
F2:33: }

F3 DGSubsetTerrainContact.c SHA256 f2cfc65f634e086af116688e287ae1694f9dca3b324985a6e7fdda5045fe54be
F3:43: bool TCTerrainAwareStepClear(vector next,out float lift,array<vector> contacts,float proposedHeading=10000)
F3:44:     {
F3:45:         if(proposedHeading==10000)proposedHeading=Body.Agent.Heading;
F3:46:         vector start=Body.Agent.Position;string rejection;
F3:47:         PressureEscapeDiagnostic="CORE";
F3:48:         if(!Body.Director.Crowd.CoreClear(Body,start,next,rejection))return false;
F3:49:         PressureEscapeDiagnostic="RESERVATION";
F3:50:         if(!Body.Director.Traffic.GroundReservationsClear(Body,start,next))return false;
F3:51:         // Airborne lateral motion must not plant a foot on an adjacent roof.
F3:52:         // Keep the existing bounded render lift, adding only terrain clearance.
F3:53:         lift=Body.Lift;
F3:54:         for(int foot=2;foot<Body.JointIds.Count();foot++)
F3:55:         {
F3:56:             vector raw=Body.JointWorldPoint(foot,next,proposedHeading,0);
F3:57:             Object terrain;vector floor,normal;float fraction;
F3:58:             if(DayZPhysics.RayCastBullet(raw+"0 0.3 0",raw+"0 -1.2 0",PhxInteractionLayers.TERRAIN,Body.Leader,terrain,floor,normal,fraction) && normal[1]>=0.65)
F3:59:                 lift=Math.Max(lift,floor[1]+Body.Radii[foot]+0.005 - raw[1]);
F3:60:         }
F3:61:         if(lift>0.28){PressureEscapeDiagnostic="LIFT_LIMIT";return false;}
F3:62:         ref array<vector> oldPoints={};ref array<vector> newPoints={};ref array<float> radii={};
F3:63:         array<float> heights={0.35,0.9,1.5};
F3:64:         foreach(float height:heights){oldPoints.Insert(start+Vector(0,height,0));newPoints.Insert(next+Vector(0,height,0));radii.Insert(0.23);}
F3:65:         contacts.Clear();
F3:66:         for(int joint=0;joint<Body.JointIds.Count();joint++)
F3:67:         {
F3:68:             vector oldJoint=Body.JointWorldPoint(joint,start,Body.Agent.Heading,Body.Lift);
F3:69:             vector newJoint=Body.JointWorldPoint(joint,next,proposedHeading,lift);
F3:70:             oldPoints.Insert(oldJoint);newPoints.Insert(newJoint);radii.Insert(Body.Radii[joint]);
F3:71:             contacts.Insert(newJoint);
F3:72:         }
F3:73:         for(int probe=0;probe<oldPoints.Count();probe++)
F3:74:         {
F3:75:             foreach(Object registered:Fixture().Objects)
F3:76:             {
F3:77:                 if(!registered)continue;
F3:78:                 float before=FixtureClearance(registered,oldPoints[probe],radii[probe]);
F3:79:                 float after=FixtureClearance(registered,newPoints[probe],radii[probe]);
F3:80:                 // Existing conservative-volume overlap may slide or leave;
F3:81:                 // a new overlap or a deeper one is never introduced.
F3:82:                 if(after<Math.Min(before,0) - 0.0001)
F3:83:                 {
F3:84:                     PressureEscapeRejectProbe=probe;PressureEscapeRejectFixture=registered.GetType();
F3:85:                     PressureEscapeRejectFixturePosition=registered.GetPosition();PressureEscapeRejectOldLift=Body.Lift;
F3:86:                     PressureEscapeRejectFixtureIndex=Fixture().Objects.Find(registered);
F3:87:                     PressureEscapeRejectAt=Now();PressureEscapeRejectBefore=before;PressureEscapeRejectAfter=after;
F3:88:                     PressureEscapeRejectLift=lift;PressureEscapeRejectRadius=radii[probe];
F3:89:                     PressureEscapeRejectStart=oldPoints[probe];PressureEscapeRejectEnd=newPoints[probe];PressureEscapeRejectIdleOffset=Body.IdleOffset;
F3:90:                     PressureEscapeRejectFraction=-1;PressureEscapeRejectNormal=vector.Zero;
F3:91:                     PressureEscapeDiagnostic="DEEPER_FIXTURE";return false;
F3:92:                 }
F3:93:             }
F3:94:             Object hit;vector point,hitNormal;float hitFraction;
F3:95:             if(DayZPhysics.SphereCastBullet(oldPoints[probe],newPoints[probe],radii[probe],DGBGroundAgent.Walls(),Body.Leader,hit,point,hitNormal,hitFraction))
F3:96:             {
F3:97:                 bool oldOverlap=hit && Fixture().Objects.Find(hit)>=0 && FixtureClearance(hit,oldPoints[probe],radii[probe])<0;
F3:98:                 // Bullet can report an initial overlap outside a conservative
F3:99:                 // object box. Require native start-contact and outward/tangent
F3:100:                 // travel as well as the non-deepening box checks above.
F3:101:                 if(!oldOverlap && hit && Fixture().Objects.Find(hit)>=0 && hitFraction>=0 && hitFraction<=0.001 && hitNormal.Length()>0.5 && vector.Dot(newPoints[probe] - oldPoints[probe],hitNormal)>=-0.000001)
F3:102:                 {oldOverlap=true;NativeStartOverlapChecks++;}
F3:103:                 bool terrainExit=false;
F3:104:                 if(!hit && probe>=5)
F3:105:                 {
F3:106:                     Object floorObject;vector floorPoint,floorNormal;float floorFraction;
F3:107:                     if(DayZPhysics.RayCastBullet(newPoints[probe]+"0 0.3 0",newPoints[probe]+"0 -1.2 0",DGBGroundAgent.Floors(),Body.Leader,floorObject,floorPoint,floorNormal,floorFraction))
F3:108:                     {
F3:109:                         vector oldProbe=oldPoints[probe];vector newProbe=newPoints[probe];
F3:110:                         terrainExit=!floorObject && floorNormal[1]>=0.65 && newProbe[1] - radii[probe]>=floorPoint[1]+0.004 && newProbe[1]>=oldProbe[1];
F3:111:                     }
F3:112:                 }
F3:113:                 if(!oldOverlap && !terrainExit && probe>=5 && hit)
F3:114:                 {
F3:115:                     float terrainGap;
F3:116:                     if(DGSubsetTerrainContactProof.InitialExit(Body.Leader,hit,oldPoints[probe],newPoints[probe],radii[probe],hitFraction,hitNormal,terrainGap))
F3:117:                     {terrainExit=true;TCLog(probe,oldPoints[probe],newPoints[probe],radii[probe],terrainGap);}
F3:118:                 }
F3:119:                 if(!oldOverlap && !terrainExit)
F3:120:                 {
F3:121:                     PressureEscapeRejectProbe=probe;PressureEscapeRejectFixture="terrain";
F3:122:                     PressureEscapeRejectFixturePosition=vector.Zero;
F3:123:                     PressureEscapeRejectFixtureIndex=-1;
F3:124:                     if(hit){PressureEscapeRejectFixture=hit.GetType();PressureEscapeRejectFixturePosition=hit.GetPosition();PressureEscapeRejectFixtureIndex=Fixture().Objects.Find(hit);}
F3:125:                     PressureEscapeRejectAt=Now();PressureEscapeRejectOldLift=Body.Lift;
F3:126:                     PressureEscapeRejectBefore=FixtureClearance(hit,oldPoints[probe],radii[probe]);
F3:127:                     PressureEscapeRejectAfter=FixtureClearance(hit,newPoints[probe],radii[probe]);
F3:128:                     PressureEscapeRejectLift=lift;PressureEscapeRejectRadius=radii[probe];
F3:129:                     PressureEscapeRejectStart=oldPoints[probe];PressureEscapeRejectEnd=newPoints[probe];PressureEscapeRejectIdleOffset=Body.IdleOffset;
F3:130:                     PressureEscapeRejectFraction=hitFraction;PressureEscapeRejectNormal=hitNormal;
F3:131:                     PressureEscapeDiagnostic="NEW_WORLD_CONTACT";return false;
F3:132:                 }
F3:133:             }
F3:134:         }
F3:135:         PressureEscapeDiagnostic="NONDEEPENING_CLEAR";return true;
F3:136:     }

Selection: five NEW_WORLD_CONTACT core rows of latest actor60 FOLLOW plan590.
[
  {
    "sourceLine": 1299,
    "seconds": "299.466",
    "id": "60",
    "phase": "FOLLOW",
    "plan": "590",
    "direction": "1",
    "position": "<4511.504395, 339.171906, 10236.789063>",
    "heading": "-9.83054",
    "dt": "0.05",
    "outcome": "NEW_WORLD_CONTACT",
    "target": "<4511.504395, 339.169891, 10236.289063>",
    "probe": "1",
    "fixture": "StaticObj_Container_1C",
    "fixturePosition": "<4510.000000, 338.823303, 10240.000000>",
    "registeredFixtureIndex": "0",
    "before": "0.0613742",
    "after": "0.0714334",
    "fraction": "0.656634",
    "normal": "<0.861558, -0.437841, 0.256930>",
    "start": "<4511.504395, 340.071899, 10236.789063>",
    "end": "<4511.504395, 340.071899, 10236.758789>"
  },
  {
    "sourceLine": 1300,
    "seconds": "299.466",
    "id": "60",
    "phase": "FOLLOW",
    "plan": "590",
    "direction": "2",
    "position": "<4511.504395, 339.171906, 10236.789063>",
    "heading": "-9.83054",
    "dt": "0.05",
    "outcome": "NEW_WORLD_CONTACT",
    "target": "<4514.504395, 339.227905, 10236.789063>",
    "probe": "0",
    "fixture": "StaticObj_Container_1C",
    "fixturePosition": "<4513.000000, 340.808350, 10240.000000>",
    "registeredFixtureIndex": "1",
    "before": "0.0635533",
    "after": "0.0351848",
    "fraction": "0.826095",
    "normal": "<-0.948473, -0.013492, -0.316571>",
    "start": "<4511.504395, 339.521912, 10236.789063>",
    "end": "<4511.534180, 339.521912, 10236.789063>"
  },
  {
    "sourceLine": 1301,
    "seconds": "299.466",
    "id": "60",
    "phase": "FOLLOW",
    "plan": "590",
    "direction": "3",
    "position": "<4511.504395, 339.171906, 10236.789063>",
    "heading": "-9.83054",
    "dt": "0.05",
    "outcome": "NEW_WORLD_CONTACT",
    "target": "<4508.004395, 339.106873, 10236.789063>",
    "probe": "0",
    "fixture": "StaticObj_Container_1C",
    "fixturePosition": "<4510.000000, 338.823303, 10240.000000>",
    "registeredFixtureIndex": "0",
    "before": "0.0519923",
    "after": "0.0237489",
    "fraction": "0.433271",
    "normal": "<0.948549, 0.016861, -0.316180>",
    "start": "<4511.504395, 339.521912, 10236.789063>",
    "end": "<4511.474609, 339.521912, 10236.789063>"
  },
  {
    "sourceLine": 1304,
    "seconds": "299.466",
    "id": "60",
    "phase": "FOLLOW",
    "plan": "590",
    "direction": "6",
    "position": "<4511.504395, 339.171906, 10236.789063>",
    "heading": "-9.83054",
    "dt": "0.05",
    "outcome": "NEW_WORLD_CONTACT",
    "target": "<4511.857910, 339.177124, 10236.435547>",
    "probe": "2",
    "fixture": "StaticObj_Container_1C",
    "fixturePosition": "<4513.000000, 340.808350, 10240.000000>",
    "registeredFixtureIndex": "1",
    "before": "0.0479225",
    "after": "0.0360224",
    "fraction": "0.599072",
    "normal": "<-0.928248, -0.013121, -0.371731>",
    "start": "<4511.504395, 340.671906, 10236.789063>",
    "end": "<4511.525391, 340.671906, 10236.767578>"
  },
  {
    "sourceLine": 1305,
    "seconds": "299.466",
    "id": "60",
    "phase": "FOLLOW",
    "plan": "590",
    "direction": "7",
    "position": "<4511.504395, 339.171906, 10236.789063>",
    "heading": "-9.83054",
    "dt": "0.05",
    "outcome": "NEW_WORLD_CONTACT",
    "target": "<4511.150879, 339.163910, 10236.435547>",
    "probe": "1",
    "fixture": "StaticObj_Container_1C",
    "fixturePosition": "<4510.000000, 338.823303, 10240.000000>",
    "registeredFixtureIndex": "0",
    "before": "0.0613742",
    "after": "0.0486761",
    "fraction": "0",
    "normal": "<0.891109, -0.453788, 0.001252>",
    "start": "<4511.504395, 340.071899, 10236.789063>",
    "end": "<4511.483398, 340.071899, 10236.767578>"
  }
]

Same-plan CPU pose,original TSV line number retained:
{
  "sourceLine": 786,
  "seconds": "299.466",
  "id": "60",
  "position": "<4511.504395, 339.171906, 10236.789063>",
  "heading": "-9.83054",
  "lift": "0.032959",
  "idleOffset": "<-0.057440, 0.000000, 0.042833>",
  "hipsMS": "<0.000000, 0.818751, 0.000000>",
  "headMS": "<-0.086715, 1.147758, -0.606594>",
  "leftFootMS": "<0.197794, 0.096155, -0.397011>",
  "rightFootMS": "<-0.306493, 0.099342, 0.327834>",
  "nativeSeconds": "220.959",
  "nativeRequest": "0",
  "phase": "FOLLOW",
  "area": "ground",
  "egressGoal": "<4511.504395, 339.172150, 10236.849609>"
}

Verified diagnostic summaries (input hash paths excluded;original local reports retain them):
[
  {
    "validation": "controlled-actor60-core-replay-first",
    "verificationStatus": "primitive_diagnostic_success_not_agent_or_path_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "sourceEntityId": 60,
    "primitiveRows": 313,
    "originalMatches": 5,
    "angularDirections": 32,
    "distancesMetres": [
      0.005,
      0.015,
      0.03
    ],
    "clearThreeCoreDirections": {
      "0": [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16
      ],
      "1": [
        0,
        1,
        12,
        13,
        14,
        15,
        16
      ],
      "2": [
        13,
        14,
        15
      ]
    },
    "originalEightClearAt30mm": [],
    "additionalClearAt30mm": [
      {
        "direction": 13,
        "degrees": 146.25
      },
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "zeroLengthControls": [
      {
        "case": "0",
        "hit": "false"
      },
      {
        "case": "1",
        "hit": "false"
      },
      {
        "case": "2",
        "hit": "false"
      },
      {
        "case": "3",
        "hit": "false"
      },
      {
        "case": "4",
        "hit": "false"
      }
    ],
    "fullBodyClearanceTested": false,
    "movementCommitted": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Three core spheres only;full bone contacts,landing footprint,peers and actual trajectory not verified.",
      "False-hit rows can contain stale native output fields;only hit boolean is interpreted.",
      "First-order test moved before other geometry smokes to avoid stale deleted fixture owners."
    ]
  },
  {
    "validation": "controlled-actor60-core-replay-first-repeat",
    "verificationStatus": "primitive_diagnostic_success_not_agent_or_path_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "sourceEntityId": 60,
    "primitiveRows": 313,
    "originalMatches": 5,
    "angularDirections": 32,
    "distancesMetres": [
      0.005,
      0.015,
      0.03
    ],
    "clearThreeCoreDirections": {
      "0": [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16
      ],
      "1": [
        0,
        1,
        12,
        13,
        14,
        15,
        16
      ],
      "2": [
        13,
        14,
        15
      ]
    },
    "originalEightClearAt30mm": [],
    "additionalClearAt30mm": [
      {
        "direction": 13,
        "degrees": 146.25
      },
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "zeroLengthControls": [
      {
        "case": "0",
        "hit": "false"
      },
      {
        "case": "1",
        "hit": "false"
      },
      {
        "case": "2",
        "hit": "false"
      },
      {
        "case": "3",
        "hit": "false"
      },
      {
        "case": "4",
        "hit": "false"
      }
    ],
    "fullBodyClearanceTested": false,
    "movementCommitted": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Three core spheres only;full bone contacts,landing footprint,peers and actual trajectory not verified.",
      "False-hit rows can contain stale native output fields;only hit boolean is interpreted.",
      "First-order test moved before other geometry smokes to avoid stale deleted fixture owners."
    ]
  },
  {
    "validation": "controlled-actor60-pose-replay",
    "verificationStatus": "frozen_pose_diagnostic_success_not_live_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "rows": 96,
    "gateCounts": {
      "NEW_WORLD_CONTACT": 88,
      "NONDEEPENING_CLEAR": 8
    },
    "fullClear30mmDirections": [
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "originalEightClearAt30mm": [],
    "frozenTrajectories": [],
    "deployedSourceOrPboChanged": false,
    "liveRegressionPassed": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Same-plan CPU pose frozen;not actual animation or 64-peer replay.",
      "False/accepted queries may leave old rejection fields;only rejection fields of rejected queries are interpreted.",
      "Actual 3D rounded steps may exceed nominal horizontal30mm;no strict30mm world-distance claim.",
      "Safe frozen path does not prove live route success or cost reduction."
    ]
  },
  {
    "validation": "controlled-actor60-pose-replay-trajectory",
    "verificationStatus": "frozen_pose_diagnostic_success_not_live_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "rows": 96,
    "gateCounts": {
      "NEW_WORLD_CONTACT": 88,
      "NONDEEPENING_CLEAR": 8
    },
    "fullClear30mmDirections": [
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "originalEightClearAt30mm": [],
    "frozenTrajectories": [
      {
        "case": 0,
        "degrees": 180.0,
        "rows": 1,
        "acceptedSteps": 0,
        "remainingMetres": 0.5,
        "travelledMetres": 0.0,
        "maximumActual3DStepMetres": 0,
        "reachedOriginal40mmGoalRadius": false,
        "lastGate": "NEW_WORLD_CONTACT"
      },
      {
        "case": 1,
        "degrees": 157.5,
        "rows": 4,
        "acceptedSteps": 3,
        "remainingMetres": 0.411134,
        "travelledMetres": 0.08887,
        "maximumActual3DStepMetres": 0.029749187232572884,
        "reachedOriginal40mmGoalRadius": false,
        "lastGate": "NEW_WORLD_CONTACT"
      },
      {
        "case": 2,
        "degrees": 168.75,
        "rows": 8,
        "acceptedSteps": 7,
        "remainingMetres": 0.290727,
        "travelledMetres": 0.209139,
        "maximumActual3DStepMetres": 0.029877312614394393,
        "reachedOriginal40mmGoalRadius": false,
        "lastGate": "NEW_WORLD_CONTACT"
      }
    ],
    "deployedSourceOrPboChanged": false,
    "liveRegressionPassed": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Same-plan CPU pose frozen;not actual animation or 64-peer replay.",
      "False/accepted queries may leave old rejection fields;only rejection fields of rejected queries are interpreted.",
      "Actual 3D rounded steps may exceed nominal horizontal30mm;no strict30mm world-distance claim.",
      "Safe frozen path does not prove live route success or cost reduction."
    ]
  },
  {
    "validation": "controlled-actor60-pose-replay-rolling-typed",
    "verificationStatus": "frozen_pose_diagnostic_success_not_live_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "rows": 96,
    "gateCounts": {
      "NEW_WORLD_CONTACT": 88,
      "NONDEEPENING_CLEAR": 8
    },
    "fullClear30mmDirections": [
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "originalEightClearAt30mm": [],
    "frozenTrajectories": [],
    "rollingComparisons": [
      {
        "mode": 0,
        "rows": 600,
        "movedSteps": 0,
        "turnSteps": 2,
        "displacementMetres": 0.0,
        "travelledMetres": 0.0,
        "maximumActual3DStepMetres": 0,
        "supported": true,
        "reachedDiagnosticHalfMetreDisplacement": false,
        "refinementPlans": 0,
        "refinementTargets": 0,
        "refinementSweeps": 0,
        "refinementAccepted": 0
      },
      {
        "mode": 1,
        "rows": 29,
        "movedSteps": 17,
        "turnSteps": 0,
        "displacementMetres": 0.507013,
        "travelledMetres": 0.512701,
        "maximumActual3DStepMetres": 0.030274245821144283,
        "supported": true,
        "reachedDiagnosticHalfMetreDisplacement": true,
        "refinementPlans": 1,
        "refinementTargets": 4,
        "refinementSweeps": 2,
        "refinementAccepted": 1
      }
    ],
    "deployedSourceOrPboChanged": false,
    "liveRegressionPassed": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Same-plan CPU pose frozen;not actual animation or 64-peer replay.",
      "False/accepted queries may leave old rejection fields;only rejection fields of rejected queries are interpreted.",
      "Actual 3D rounded steps may exceed nominal horizontal30mm;no strict30mm world-distance claim.",
      "Safe frozen path does not prove live route success or cost reduction."
    ]
  },
  {
    "validation": "controlled-actor60-pose-replay-rolling-typed-repeat",
    "verificationStatus": "frozen_pose_diagnostic_success_not_live_acceptance",
    "sourceRunId": "runtime-20261030-084554",
    "rows": 96,
    "gateCounts": {
      "NEW_WORLD_CONTACT": 88,
      "NONDEEPENING_CLEAR": 8
    },
    "fullClear30mmDirections": [
      {
        "direction": 14,
        "degrees": 157.5
      },
      {
        "direction": 15,
        "degrees": 168.75
      }
    ],
    "originalEightClearAt30mm": [],
    "frozenTrajectories": [],
    "rollingComparisons": [
      {
        "mode": 0,
        "rows": 600,
        "movedSteps": 0,
        "turnSteps": 2,
        "displacementMetres": 0.0,
        "travelledMetres": 0.0,
        "maximumActual3DStepMetres": 0,
        "supported": true,
        "reachedDiagnosticHalfMetreDisplacement": false,
        "refinementPlans": 0,
        "refinementTargets": 0,
        "refinementSweeps": 0,
        "refinementAccepted": 0
      },
      {
        "mode": 1,
        "rows": 29,
        "movedSteps": 17,
        "turnSteps": 0,
        "displacementMetres": 0.507013,
        "travelledMetres": 0.512701,
        "maximumActual3DStepMetres": 0.030274245821144283,
        "supported": true,
        "reachedDiagnosticHalfMetreDisplacement": true,
        "refinementPlans": 1,
        "refinementTargets": 4,
        "refinementSweeps": 2,
        "refinementAccepted": 1
      }
    ],
    "deployedSourceOrPboChanged": false,
    "liveRegressionPassed": false,
    "visualAcceptance": false,
    "performanceAcceptance": false,
    "limitations": [
      "Same-plan CPU pose frozen;not actual animation or 64-peer replay.",
      "False/accepted queries may leave old rejection fields;only rejection fields of rejected queries are interpreted.",
      "Actual 3D rounded steps may exceed nominal horizontal30mm;no strict30mm world-distance claim.",
      "Safe frozen path does not prove live route success or cost reduction."
    ]
  }
]

Not proved: live recovery,64/64,visible animation quality,20ms p95 or CPU/GPU cost improvement.

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
