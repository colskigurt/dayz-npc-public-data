공개 읽기 미러: https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main
실제 게시/접근 검증: https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/verification/latest.json

# DayZ NPC — 다른 AI를 위한 단일 검토 문서

전체 원시 로그·영상이 아닌 기존 공개 보고서와 선정된 함수/로그 발췌입니다. 아래 자료는 실시간이 아니며 최종 수용 전입니다. 영상만으로 확인할 품질을 텍스트만으로 성공 판정하지 마세요.

# DayZ NPC 개발·진단 현황 — 공개 읽기 스냅샷

schemaVersion: 1.0.0
snapshotId: snapshot-20261027T185443179320Z-acd7bf8c
generatedAt: 2026-10-27T18:54:43.179320Z
sourceUpdatedAt: 2026-10-27T17:36:00.756451Z
staleAfter: 2026-10-28T17:36:00.756451Z

생성 시각은 실험 수행 시각이 아닙니다. 현재 UTC가 staleAfter 이후면 오래된 자료(stale)로 해석하세요.

## 1. 작업 목표
안정성·실제 화면 품질·성능을 독립 판정합니다. 원인 특정 전 새 보간이나 갱신 방식을 추측으로 적용하지 않습니다.

## 2. 실제 수행
첫 재접근 후보의8/64 실제 시험, 수정 B8FB의 엔진 회귀, 단일 NPC CPU/영상 A/B/B/A 진단을 보존했습니다.

## 3. 측정 결과
|조건|최종 도착|HIGH_HOLD 내 미도달|낙하 복귀|미션 p50/p95/p99|새 분기|
|---|---:|---:|---:|---|---:|
|8명·runtime-20261028-021529·180초|8/8|0명|10회|20.0/33.0/34.0ms|0회|
|64명·runtime-20261028-021953·300초|64/64|11명|77회|25.0/59.0/82.0ms|0회|

위 두 행은 인원/실행 길이가 달라 성능 전후 대조가 아닙니다. 각1회 상세진단이며 GPU/Present 지연이 아닙니다.
CPU run 반복 주기 A1/B1/B2/A2: 3.24/2.08/2.08/3.22초. 단일 논리 NPC/8부품, AI·이동OFF, 같은 VSyncON,15~70초 구간. A/B/B/A는 평균 시간 반응만 반복 확인했고 부드러움은 수용하지 않았습니다. 개별 잔차와 원시 수치는 지표 JSON에 있습니다.

## 4. 원인 분석
첫 후보는 Drive가 Follow 전에 이유 문자열을 지워 새 분기가 작동하지 않았습니다. B8FB는 명시적 거부 전달을 엔진 대역으로 검증했지만 실제 이동은 미실행입니다.
애니메이션의 정확한 표시 불일치 원인은 미확정입니다. 보간 입력800개에서 상한 포화가 없었으며 평균 속도 보정으로 끊김이 해결됐다는 가정도 지지되지 않았습니다.

## 5. 검증 수준
진단 성공·74개 보존 회귀 통과와 전체 실험 성공/최종 수용을 구분합니다. 현재 기능·시각 품질·성능 모두 최종 수용 전입니다. 20ms p95는 미달성 목표입니다.

## 6. 변경 사항
공개 스냅샷은 읽기 전용 정적 파일입니다. 게임 코드·PBO·실험 기록을 변경하지 않았습니다. sourceRunId/buildId/조건은 아래 JSON에서 확인할 수 있습니다.

## 7. 원시 증거 연결
이 페이지는 전체 원시 로그가 아닌 선정된 공개 근거에 연결합니다.
- [상태 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/status.json)
- [실험 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/experiments.json)
- [지표 JSON](https://dayz-npc-observatory.sappyvole8.chatgpt.site/api/public/metrics.json)
- [기능 함수·로그 패킷](https://dayz-npc-observatory.sappyvole8.chatgpt.site/downloads/admission-review.txt)
- [애니메이션 함수·로그 패킷](https://dayz-npc-observatory.sappyvole8.chatgpt.site/downloads/animation-review.txt)

## 8. 잔여 문제
B8FB의8→64 실제 이동, CPU와 표시의 시간적 대응, 별도 반복 성능 기여도 실험이 남아 있습니다. 영상/GPU/compiled clip phase는 텍스트만으로 검증할 수 없습니다.

## 클라이언트 안내
인증/JavaScript 없이 GET할 수 있도록 만들었습니다. 라이브러리 기본 User-Agent에서 Cloudflare1010이 재현됐으므로 정체를 밝히는 User-Agent(예: DayZNPC-PublicReader/1.0)를 권장합니다. 실제 GPTBot의 접근 성공을 뜻하지 않습니다. 모든 외부 AI 도구의 접근은 아직 보증하지 않습니다.


## 선정된 공개 근거: downloads/admission-review.txt

다른 세션용 DayZ NPC 검토 패킷

이 파일은 독립 검토용 자료다. 다른 세션이 원본 파일에 접근할 수 있다고 가정하지 않는다.
코드는 명시된 구간의 원문이며 생략 구간이 있다. 로그 일부는 명시한 필드 투영/대표 표본이다.
해시는 출처 식별용이다. 이 텍스트만으로 전체 원본 무결성이나 영상 품질을 독립 검증할 수는 없다.
관측 사실과 가설을 구분하고, 빠진 호출자·수명·물리·GPU 정보가 있으면 요청할 것.

입구 재접근 기능 진단 — 미작동 후보와 거부 전달 수정

작성: 2026-10-28T02:35:58.498698+09:00

1. 작업 목표

기록된 입구 위치와 실제 시작 위치의 차이로 출구 지지가 거부되는 문제를 재현하고, 기존 보행을 이용한 지역 재접근 후보를 검증했다. 기능 회귀 묶음이며 애니메이션/성능 최적화 실험과 분리한다.

2. 실제 수행

- 동일 컨테이너에서 실제 LandingSpace/LaneSupport와 기록 포즈를 재생했다. 지지 검사나 정상 접촉 낙하는 완화하지 않았다.
- 첫 3ECCE4F1 후보로 8명/180초, 64명/300초 클라이언트를 실행하고 화면을 확인했다. 두 실행 모두 정상 종료됐으나 새 분기는 0회였다.
- Drive가 Follow 이전에 이유 문자열을 지우는 오류를 확인했다. B8FBAB74 후보는 거부 사실을 다음 틱에 명시적으로 전달한다.
- 추가 엔진 검사는 이유 문자열 삭제 후 보행 의도 생성·위치 비변경·목표 변경 취소를 확인했다. 이 검사에서 보행 실행은 대역이며 실제 이동 성공이 아니다.
- 높은 구역 성공 ID를 전체 시간이 아닌 HIGH_HOLD 표본 구간에서 집계하도록 분석기를 고쳤다. 원시 자료와 과거 보고서는 보존했다.

3. 측정 결과

|항목|첫 후보 8명·021529|첫 후보 64명·021953|
|---|---:|---:|
|최종 도착|8/8|64/64|
|낙하 후 FOLLOW 복귀|10|77|
|HIGH_HOLD 내 미도달|0|11|
|출구 복구 lease 위반|0|0|
|새 재접근 분기|0|0|
|미션 frame-time p50/p95/p99|20/33/34ms|25/59/82ms|

이 frame-time은 미션 갱신 간격이며 GPU/Present 지연이 아니다. 인원·실행 길이가 다르고 상세 계측을 포함하므로 두 열의 성능을 비교하지 않는다. 반복 성능 개선/20ms p95 달성은 미검증이다. 이번 시각 확인은 장면 존재·동작 관찰이며 애니메이션 끊김의 정량 수용이 아니다.

기준 3BE0/214810의 HIGH_HOLD 미도달은 9·13·51번이다. 9/51의 첫 높은 면 FOLLOW는 107.453/105.872초로 유지 구간 마지막 표본 104.951초 이후였다. 기존 최종 도착 64/64는 유효하다. 첫 후보 64명의 구간 미도달 ID는 1,15,19,23,31,35,36,39,43,47,54다.

4. 원인 분석

사실: 기록 위치의 출구는 FOOT_X_SUPPORT 실패, 약 0.156m 옆의 선택 입구에 정렬한 가정은 CLEAR였다. 첫 후보는 원래 Drive의 문자열 초기화 때문에 시작 조건을 소비하지 못했다.

추정/미확정: 명시적 거부 전달과 재접근이 실제 혼잡에서 높은 면 도달률을 높일지는 아직 모른다. 모든 낙하·장시간 복구를 입구 오차 하나로 설명하지 않는다.

반증: 첫 후보의 정적 선택 검사 통과가 실제 분기 작동을 입증한다는 가정은 실제 실행에서 반증됐다. 마지막 거부를 단순 개체 예약 문제로 해석하는 것은 FOOT_X_SUPPORT와 맞지 않는다.

5. 검증 수준

|대상|수준|판정 범위|
|---|---|---|
|기록 포즈의 출구 불일치|진단 성공|실제 엔진의 정적 지지 검사|
|첫 후보 8/64 실행|진단 성공|정상 종료·분기 미작동 확인, 개선 성공 아님|
|B8FB 거부 전달·취소|회귀 통과|엔진 대역 검사, 실제 보행은 미실행|
|기존 NPC 증거 회귀|회귀 통과|74개 검사, 전체 기능 합격과 별개|
|B8FB 8/64 이동|미실행|다음 기능 실험|
|반복 기능 효과/시각 품질/성능|미실행|최종 수용 없음|

6. 변경 사항

- tests/dg_native_probe/subset_footprint_recovery.c: opt-in 지역 접근과 틱 간 거부 전달. 기존 Walk/SurfaceWalk/접촉 검사 유지.
- tools/run_horde_controlled_investigation.ps1: AdmissionReapproach 플래그와 실행 기록.
- tools/compile_days_gone_native_runtime_probe.ps1: 전용 진단 미션과 완료 marker 검사.
- tools/analyze_horde_missed_surface.py: HIGH_HOLD 안의 성공만 집계.
- 첫 후보 3ECCE4F140E8A5A7F1081D657EEFA3EB875717D9050E0554F23C94E1ABE941EC는 미작동 후보로 보존했다.
- 다음 후보 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185는 빌드·서명·서명 검증·엔진 검사 성공, 실제 이동 미검증이다. 원본 453D와 자산 0C6E는 변경하지 않았다. 개인 키는 배포물에 포함하지 않았다.

7. 원시 증거

- 기존 상세 보고서 (ADMISSION_REAPPROACH_2026-10-28.md)
- 시간 구간 집계 대조 JSON (HIGH_COVERAGE_WINDOW_2026-10-28.json)
- 8명 실행 폴더 (runtime-20261028-021529), 64명 실행 폴더 (runtime-20261028-021953): launch.json, native-ground.tsv, native-state-contract.tsv, native-traversal-events.tsv, state-contract-report.json, admission-reapproach-analysis.json. 이번 실행 영상은 녹화하지 않았다.
- B8FB 소스·PBO·서명·엔진 증거 manifest (manifest.json) 및 candidate-engine-compile.json의 검사 profile.

재현: 같은 Chernarus 컨테이너 미션/그래픽 seed/VSync ON에서 activeReference, StateContract, DriveAttribution, EgressRange=extended, EgressFirstStep, EgressNearFirstStep, EndpointLookahead, TerrainContactOwner, PursuitPathRevalidation, AdmissionReapproach를 사용했다. 인원/초는 표와 같으며 launch.json에 전체 조건이 있다.

8. 잔여 문제

B8FB의 8명→64명 실제 이동, 분기 실행과 완료/취소 확인, 같은 PBO의 플래그 OFF/ON 반복 대조가 남았다. 정상 낙하는 유지하고 구간 미도달을 숨기지 않는다. 이 묶음으로 성능 개선이나 화면 부드러움을 수용하지 않는다. 진행 기록 복구는 종료·보존하며 그 작업은 재개하지 않는다.

원본 보고서 ID: 2026-10-28_023558_498698+0900_npc-admission-functional.md
원본 SHA-256: a2903bfc8fcc146e64bdffc3cf40d602074bb45ba1697f55a879c1ffc3c4d7c0
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

===== 추가 기술 근거 =====
[F1] 실제 원본 PBO 구성원과 바이트 일치를 확인한 기반 Drive 진입부. 전체 함수 아님.

SOURCE: DGBNativeTraversal.c; lines 1351-1363; SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872
1351:     void Drive(float dt,vector delta,bool available)
1352:     {
1353:         Body.Agent.MeasuredSpeed=0;Body.Reason="";Body.HitType="";Body.HitPoint=vector.Zero;
1354:         Body.HitNormal=vector.Zero;Body.HitFraction=-1;
1355:         int acceptedBeforeDrive=Body.AcceptedSteps;
1356:         if(Phase=="FOLLOW")Follow(dt,delta,available);
1357:         else if(Phase=="MOTION")Motion(dt,delta,available);
1358:         else if(Phase=="FALL"){Body.NativeFallExecutions++;Fall(dt);}
1359:         else if(Phase=="LAND" || Phase=="SETTLE")Settle(dt);
1360:         else if(Phase=="EGRESS")Egress(dt,delta,available);
1361:         // A source idle pose can touch a fixture after an otherwise valid
1362:         // traversal/combat endpoint. Recover on the same supported surface,
1363:         // using the same swept non-deepening checks; never during action locks.

[F2] 기존 Follow의 입구 정렬/출구 재계산 구간. 전체 함수 아님.

SOURCE: DGBNativeTraversal.c; lines 625-649; SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872
625:         {Body.NextRequest=0;Body.Reason="ENTRY_POSE_SETTLE";MotionClearanceDiagnostic="ENTRY_POSE_SETTLE";return;}
626:         // Fit the immutable source curve to the accepted actual start/heading.
627:         // Validate its resulting landing before starting; never snap to an exit.
628:         if(unreserved)
629:         {
630:             if(!StartPlacement || vector.Distance(StartPlacement.Entry,agent.Position)>0.005 || Math.AbsFloat(DGBGroundAgent.SignedAngle(StartPlacement.Heading-agent.Heading))>0.1)
631:                 StartPlacement=DGBNativeAdmission.Place(desired,Body.Id,agent.Position,agent.Heading);
632:             desired=StartPlacement;
633:         }
634:         vector exitSupport;
635:         bool destinationReady=LandingSpace(desired.Exit,exitSupport,!unreserved);
636:         if(destinationReady && Fixture().Area(exitSupport)!=desired.TargetArea)
637:         {destinationReady=false;LandingDiagnostic="TARGET_AREA_MISMATCH";}
638:         if(destinationReady && unreserved && !Fixture().LaneSupport(desired.Exit,desired.TargetArea))
639:         {destinationReady=false;LandingDiagnostic="TARGET_FOOTPRINT_AREA";}
640:         bool capacityReady=true;
641:         if(!unreserved && destinationReady && desired.TargetArea!="ground")capacityReady=Body.Director.Traffic.SurfaceSeat(Body,desired.TargetArea,true)!=null;
642:         if(!destinationReady || !capacityReady)
643:         {
644:             Body.Reason="EXIT_SPACE_BLOCKED";if(!capacityReady)Body.Reason="TARGET_CAPACITY_WAIT";
645:             if(ExitBlockedAt<0)ExitBlockedAt=Now();
646:             if(Now() - ExitBlockedAt>3)Backoff("EXIT_LEASE_BACKOFF");
647:             return;
648:         }
649:         if(desired.Action=="DROP" && agent.Position[1] - exitSupport[1]>3.5)

[F3] 첫 미작동 후보의 ARFollow 전체. Drive에서 Reason을 지운 후 이 함수가 호출된다.

SOURCE: DGSubsetFootprintRecovery.c; lines 56-84; SHA256 529716648581d8ba986f0cf57dfdac5ecb7451cb5eac602a23c3ea609ee924d6
56:     bool ARFollow(float dt,vector delta,bool available)
57:     {
58:         if(!ARPolicyRead){AREnabled=FileExist("$profile:admission-reapproach.flag");ARPolicyRead=true;}
59:         if(!AREnabled)return false;
60:         if(!AROwnershipAvailable())
61:         {if(ARActive)ARFinish("SUCCESSOR_OWNS_MOTION",false);return false;}
62:         if(ARActive && (GoalRevision!=ARRevision || Area!=ARArea || Active!=ARSource))
63:         {ARFinish("INTENT_CHANGED");return false;}
64:         if(ARActive && Now()>=ARUntil){ARFinish("EXPIRED");return false;}
65:         if(!ARActive)
66:         {
67:             if(Now()<ARNextCheck || Body.Reason!="EXIT_SPACE_BLOCKED")return false;
68:             if(LandingDiagnostic!="FOOT_X_SUPPORT" && LandingDiagnostic!="FOOT_Z_SUPPORT" && LandingDiagnostic!="TARGET_FOOTPRINT_AREA")return false;
69:             ARNextCheck=Now()+0.25;vector goal;
70:             if(!ARProposal(goal))return false;
71:             ARGoal=goal;ARRevision=GoalRevision;ARArea=Area;ARSource=Active;
72:             ARUntil=Now()+4;ARActive=true;EntryAligning=false;
73:             Body.Agent.Path.Clear();Body.Agent.PlanAt=0;Body.RetryAt=0;
74:             SurfaceWaypoints.Clear();NextSurfacePlan=0;ARLog("START");
75:         }
76:         if(DGBGroundAgent.FlatDistance(Body.Agent.Position,ARGoal)<=0.045)
77:         {ARFinish("ARRIVED");return false;}
78:         // Native walk delta, heading and collision gates remain authoritative.
79:         SafetyWalk=true;
80:         if(Area=="ground")Walk(ARGoal,0.04,false,dt,delta,available);
81:         else SurfaceWalk(ARGoal,0.04,dt,delta,available);
82:         Body.NextRequest=1;
83:         return true;
84:     }

[F4] 수정 B8FB의 지역 재접근 함수군. Drive 오버라이드, 조건, 취소, 보행 호출 포함.

SOURCE: DGSubsetFootprintRecovery.c; lines 1-98; SHA256 6e3bb6f1b1e7b232bed0aa9f9d623e7a8f022a27463f403cf683fb64acfb8fd3
1: // Local roof safety intent, not player evidence. All root motion still runs
2: // through Walk -> DriveGround -> MoveTo -> SolveContacts unchanged.
3: modded class DGBNativeTraversal
4: {
5:     bool FRActive;bool FRTurning;bool FRPolicyRead;bool FRPolicyEnabled;
6:     float FRBlockedAt=-1;float FRNextPlan;float FRUntil;
7:     vector FRGoal;vector FROrigin;string FRArea;string FRLastStatus;
8:     int FRPlans;int FRStarts;int FRCompleted;int FRRejected;int FRTurns;
9:     static float FRBudgetAt=-1;static int FRPlansThisFrame;
10:     bool ARPolicyRead;bool AREnabled;bool ARActive;
11:     float ARUntil;float ARNextCheck;int ARRevision;string ARArea;
12:     vector ARGoal;DGBFixtureLink ARSource;
13:     bool ARStartRejected;int ARRejectedRevision;DGBFixtureLink ARRejectedSource;
14:     // Drive clears Body.Reason before Follow, so capture the completed
15:     // previous decision explicitly rather than reading a stale display field.
16:     void ARCaptureOutcome()
17:     {
18:         ARStartRejected=AREnabled && !ARActive && Phase=="FOLLOW" && Body.Reason=="EXIT_SPACE_BLOCKED";
19:         if(ARStartRejected)
20:         {ARRejectedRevision=GoalRevision;ARRejectedSource=Active;}
21:         else ARRejectedSource=null;
22:     }
23:     override void Drive(float dt,vector delta,bool available)
24:     {
25:         super.Drive(dt,delta,available);ARCaptureOutcome();
26:     }
27:     // A separate local intent only after the ordinary start-placement gate
28:     // rejects support. It never changes Admission/Active or any position.
29:     bool ARProposal(out vector goal)
30:     {
31:         if(!Body.Director.OpenTraversal || Phase!="FOLLOW" || !Admission || !Active || AdmissionSource!=Active)return false;
32:         if(Admission.Action!="CLIMB" || Admission.SourceArea!=Area)return false;
33:         float gap=DGBGroundAgent.FlatDistance(Body.Agent.Position,Admission.Entry);
34:         if(gap<=0.045 || gap>0.36)return false;
35:         float error=Math.AbsFloat(DGBGroundAgent.SignedAngle(Body.Agent.Heading-Admission.Heading));
36:         if(error>1)return false;
37:         string saved=LandingDiagnostic;vector support;
38:         ref DGBFixtureLink current=DGBNativeAdmission.Place(Admission,Body.Id,Body.Agent.Position,Body.Agent.Heading);
39:         bool currentValid=LandingSpace(current.Exit,support,false);
40:         if(currentValid)currentValid=Fixture().Area(support)==Admission.TargetArea && Fixture().LaneSupport(current.Exit,Admission.TargetArea);
41:         bool targetValid=false;
42:         if(!currentValid)targetValid=LandingSpace(Admission.Exit,support,false) && Fixture().Area(support)==Admission.TargetArea && Fixture().LaneSupport(Admission.Exit,Admission.TargetArea);
43:         LandingDiagnostic=saved;
44:         if(currentValid || !targetValid)return false;
45:         goal=Admission.Entry;return true;
46:     }
47:     void ARLog(string status)
48:     {
49:         FileHandle file=OpenFile("$profile:native-admission-reapproach.tsv",FileMode.APPEND);if(!file)return;
50:         FPrintln(file,(Now()-Body.Director.Born).ToString()+"\t"+Body.Id+"\t"+status+"\t"+Phase+"\t"+Body.Agent.Position+"\t"+ARGoal+"\t"+GoalRevision+"\t"+Body.Reason);
51:         CloseFile(file);
52:     }
53:     void ARFinish(string status,bool releaseLocalMotion=true)
54:     {
55:         ARLog(status);ARActive=false;ARSource=null;ARNextCheck=Now()+0.75;
56:         if(releaseLocalMotion)
57:         {
58:             SafetyWalk=false;ForceWalk=false;EntryAligning=false;StartPlacement=null;
59:             Body.Agent.Path.Clear();Body.Agent.PlanAt=0;Body.RetryAt=0;
60:             SurfaceWaypoints.Clear();NextSurfacePlan=0;NextAdmissionPlan=0;
61:         }
62:     }
63:     bool AROwnershipAvailable()
64:     {
65:         if(Phase!="FOLLOW" || FRActive || IdleClearing || SurfaceYieldActive || PoseClearanceActive)return false;
66:         if(Body.YieldActive || Body.SupportBypassActive || Body.PeerBypassActive)return false;
67:         if(Body.Combat && (Body.Combat.Active || Body.Combat.Dead || Body.Combat.PendingDeath))return false;
68:         return true;
69:     }
70:     bool ARFollow(float dt,vector delta,bool available)
71:     {
72:         if(!ARPolicyRead){AREnabled=FileExist("$profile:admission-reapproach.flag");ARPolicyRead=true;}
73:         if(!AREnabled)return false;
74:         if(!AROwnershipAvailable())
75:         {if(ARActive)ARFinish("SUCCESSOR_OWNS_MOTION",false);return false;}
76:         if(ARActive && (GoalRevision!=ARRevision || Area!=ARArea || Active!=ARSource))
77:         {ARFinish("INTENT_CHANGED");return false;}
78:         if(ARActive && Now()>=ARUntil){ARFinish("EXPIRED");return false;}
79:         if(!ARActive)
80:         {
81:             if(Now()<ARNextCheck || !ARStartRejected || ARRejectedRevision!=GoalRevision || ARRejectedSource!=Active)return false;
82:             if(LandingDiagnostic!="FOOT_X_SUPPORT" && LandingDiagnostic!="FOOT_Z_SUPPORT" && LandingDiagnostic!="TARGET_FOOTPRINT_AREA")return false;
83:             ARNextCheck=Now()+0.25;vector goal;
84:             if(!ARProposal(goal))return false;
85:             ARGoal=goal;ARRevision=GoalRevision;ARArea=Area;ARSource=Active;
86:             ARUntil=Now()+4;ARActive=true;ARStartRejected=false;EntryAligning=false;
87:             Body.Agent.Path.Clear();Body.Agent.PlanAt=0;Body.RetryAt=0;
88:             SurfaceWaypoints.Clear();NextSurfacePlan=0;ARLog("START");
89:         }
90:         if(DGBGroundAgent.FlatDistance(Body.Agent.Position,ARGoal)<=0.045)
91:         {ARFinish("ARRIVED");return false;}
92:         // Native walk delta, heading and collision gates remain authoritative.
93:         SafetyWalk=true;
94:         if(Area=="ground")Walk(ARGoal,0.04,false,dt,delta,available);
95:         else SurfaceWalk(ARGoal,0.04,dt,delta,available);
96:         Body.NextRequest=1;
97:         return true;
98:     }

[F5] 정적 엔진 재생 TSV 전체. mode0=기록 위치/heading, mode1=선택 입구 완전 정렬 가정. 실제 이동 아님.

SOURCE: recorded-admission.tsv; lines 1-5; SHA256 cd1ad2e61ca41fa5f003e7f1fe42ce9cb465f3bf64bda1c20d66309710c2e22d
1: stamp	mode	start	heading	admissionEntry	exit	support	valid	diagnostic	laneSupport
2: 88.099	0	<4508.902344, 339.118286, 10235.236328>	50.469	<4509.067383, 339.123566, 10235.276367>	<4510.416504, 340.132507, 10236.488281>	<0.000000, 0.000000, 0.000000>	false	ROOT_SUPPORT	false
3: 88.099	1	<4509.067383, 339.123566, 10235.276367>	0	<4509.067383, 339.123566, 10235.276367>	<4509.065430, 340.137787, 10237.241211>	<4509.065430, 340.168762, 10237.241211>	true	CLEAR	true
4: 102.702	0	<4508.762207, 339.115906, 10235.262695>	0	<4508.917480, 339.123566, 10235.276367>	<4508.760254, 340.130127, 10237.227539>	<4508.760254, 340.163025, 10237.227539>	false	FOOT_X_SUPPORT	false
5: 102.702	1	<4508.917480, 339.123566, 10235.276367>	0	<4508.917480, 339.123566, 10235.276367>	<4508.915527, 340.137787, 10237.241211>	<4508.915527, 340.165955, 10237.241211>	true	CLEAR	true

[F6] runtime-20261028-021529/launch.json; SHA256 87e4e6baf27dc887839f169f7f0e32a609dd10651b0ffa5692b998ced7acaae1; selected JSON fields (not complete file):
{
  "normalQuitVerified": true,
  "count": 8,
  "seconds": 180,
  "subsetIntegrationPboSha256": "3ECCE4F140E8A5A7F1081D657EEFA3EB875717D9050E0554F23C94E1ABE941EC",
  "admissionReapproach": true,
  "missionSourcesSha256": "F8356C4F1E0286DCE7ED83F0137E68061249D48ABEB175942D44A8A048D704D3",
  "frameSync": "on"
}

[F6] runtime-20261028-021529/state-contract-report.json; SHA256 7cf10a92017c6adc7b52fc899c22ef16e05028c3e7d564abc52b913ae3d2366e; selected JSON fields (not complete file):
{
  "terminalArrivals": 8,
  "fallsLandedAndResumed": 10,
  "foregroundMissionFrameMs": {
    "samples": 8748,
    "p50": 20.0,
    "p95": 33.0,
    "p99": 34.0
  },
  "newFailureLabelEpisodes": {
    "CONTACT_SUPPORT_LOST": 2
  }
}

[F6] runtime-20261028-021529/admission-reapproach-analysis.json; SHA256 5cdbea49ae86a0e831d51c000b023d4e7b83d702d7a6a182c12d87771278ce3b; selected JSON fields (not complete file):
{
  "branchExercised": false,
  "episodes": [],
  "violations": []
}

[F6] runtime-20261028-021953/launch.json; SHA256 4072dd68680e3d15266617635810610b2845a2f64e3449e723ca0a14247c9e5e; selected JSON fields (not complete file):
{
  "normalQuitVerified": true,
  "count": 64,
  "seconds": 300,
  "subsetIntegrationPboSha256": "3ECCE4F140E8A5A7F1081D657EEFA3EB875717D9050E0554F23C94E1ABE941EC",
  "admissionReapproach": true,
  "missionSourcesSha256": "F8356C4F1E0286DCE7ED83F0137E68061249D48ABEB175942D44A8A048D704D3",
  "frameSync": "on"
}

[F6] runtime-20261028-021953/state-contract-report.json; SHA256 77b88d5492d57933833444d1fcebf98fdd485ba66196d864a4f163b60f9b7f76; selected JSON fields (not complete file):
{
  "terminalArrivals": 64,
  "fallsLandedAndResumed": 77,
  "foregroundMissionFrameMs": {
    "samples": 9806,
    "p50": 25.0,
    "p95": 59.0,
    "p99": 82.0
  },
  "newFailureLabelEpisodes": {
    "CONTACT_SUPPORT_LOST": 17
  }
}

[F6] runtime-20261028-021953/admission-reapproach-analysis.json; SHA256 cfe4c44ba971b403911fc441ab930aac30cb57250d74adc2a77739a739a13609; selected JSON fields (not complete file):
{
  "branchExercised": false,
  "episodes": [],
  "violations": []
}

[F7] 누락: 전체 개체 로그, 실제 재접근 동작(B8FB 미실행), 네이티브 물리 내부. 전체 기능 성공을 추론할 수 없다.

===== 검토 요청 =====
1. 제시한 증거로 확정할 수 있는 결론과 과도한 결론을 나눠라.
2. 반례·누락된 상태/호출 관계·측정 편향을 찾아라.
3. 원인을 구분할 다음 최소 실험과 필요한 추가 자료를 제안하라.
4. 코드 제안은 가설로 표시하고 실제 수정/수용 완료로 표현하지 말라.


## 선정된 공개 근거: downloads/animation-review.txt

다른 세션용 DayZ NPC 검토 패킷

이 파일은 독립 검토용 자료다. 다른 세션이 원본 파일에 접근할 수 있다고 가정하지 않는다.
코드는 명시된 구간의 원문이며 생략 구간이 있다. 로그 일부는 명시한 필드 투영/대표 표본이다.
해시는 출처 식별용이다. 이 텍스트만으로 전체 원본 무결성이나 영상 품질을 독립 검증할 수는 없다.
관측 사실과 가설을 구분하고, 빠진 호출자·수명·물리·GPU 정보가 있으면 요청할 것.

CPU 포즈·실제 화면 시간 진단 — 완료된 A/B/B/A 묶음

작성: 2026-10-28T02:36:00.756451+09:00

1. 작업 목표

CPU 포즈 갱신과 실제 화면의 시간적 불일치를 조사한다. 평균 clip 진행 속도와 표시 프레임 간 연속성을 분리해 판단한다. 완료된 기존 A/B/B/A 묶음을 새 보고 형식으로 연결한 것이며 이번 보고를 위해 새 보간·업데이트 방식을 적용하지 않았다.

2. 실제 수행

단일 논리 NPC/8부품, AI·이동 OFF, 고정 run clip의 같은 미션·PBO·그래픽에서 기준 A→평균 시간 보정 B→B→A를 실행했다. CPU 6본 포즈, 원본 자세 적합 proxy, 정확한 영상 HUD 프레임과 몸 마스크를 대조했다. 별도의 기존 보간 인자 기록 400개씩 두 실행도 재분석했다. 모든 실행·녹화는 정상 종료됐다.

3. 측정 결과

|실행|CPU 자세 반복 주기|속도 정규화 증가 잔차 RMS|6본 원본 적합 잔차 p95|정확한 영상/HUD 연결 프레임|같은 프레임 몸 변화/CPU 상관|
|---|---:|---:|---:|---:|---:|
|A1·013539|3.24s|24.07ms|13.80mm|360|0.92463|
|B1·013820|2.08s|24.26ms|29.00mm|357|0.94566|
|B2·014722|2.08s|24.05ms|28.80mm|360|0.94210|
|A2·015001|3.22s|23.71ms|14.56mm|348|0.92491|


공통 15~70초 전경 로그는 모두100%였다. 영상 연결은20~23초/30~33초 구간이다. 원본 run 길이는125/60=2.0833초다. proxy는1/4키 격자와6본 FK 적합이며 직접 clip 위상이나 지각적 품질 측정이 아니다. 명령 시간의8부품 최대 차이는 후보에서 약4~5ms였으나 실제 부품 위상 차이로 측정한 것은 아니다.

별도003842/004145 보간 입력: 400개씩, 완전 주기100/99개, 중앙 주기80ms, descriptor duration50ms, 비율≥1 또는<0인 입력0개. 이 표본에서 상한 포화에 머무는 현상은 관측되지 않았다.

8/64 기능 안정성: 이 묶음에서 미측정. frame-time p50/p95/p99 및 AI/경로/접촉 비용: 이 묶음은 로그·포즈 조회·녹화 진단이므로 성능 수용에서 제외했다. GPU/Present 시각·관절별 광학 시간차·buffer 교체 연속성: 미측정. 60fps 녹화는60Hz 애니메이션 평가를 뜻하지 않는다.

4. 원인 분석

사실: 평균 run 주기는 순서 교차 후에도 보정됐지만 증가 잔차는 약24ms로 남았고, 후보의 자세 적합 잔차가 더 컸다. 정확한 HUD 연결은 확보했으나 모호한 프레임은 제외했다.

추정/미확정: CPU 포즈와 실제 표시가 시간상 일치하지 않는 지점이 buffer 교체/렌더 소비에 있을 수 있다. 정확한 원인·인과는 아직 특정하지 못했다. CPU/몸 변화 상관이 높다는 사실만으로 표시 품질이나 지연을 수용하지 않는다.

반증/지지되지 않음: 평균 재생 속도만 바로잡으면 끊김도 해결된다는 가정은 이 후보 결과로 지지되지 않는다. 상한 clamp 정지 가설도 보간 입력800개에서 지지되지 않았다. 50ms descriptor와약80ms 평가 주기의 차이만으로 표시 정지를 확정할 수 없다.

5. 검증 수준

|대상|수준|독립 판정|
|---|---|---|
|CPU·영상 프레임 연결|진단 성공|한정 구간 정확한 HUD/프레임 조인|
|평균 run 시간 보정 반응|반복 검증 완료|A/B/B/A의 좁은 반응만 확인|
|평균 보정 후보의 부드러움|진단 성공|개선 입증 실패·채택하지 않음|
|표시 불일치 원인 특정|미실행|buffer/렌더 소비의 직접 인과 검증 미완료|
|애니메이션 최종 수용|미실행|전체14상태·동작 전환·64명에 확대하지 않음|
|성능 최종 수용|미실행|20ms p95는 목표이며 미달성|

6. 변경 사항

이번 게시 작업은 보고서·무결성 검증뿐이다. 기존 별도 평균 시간 보정 probe는 102757EC9542DEFBB1EE65BDD18FFC7429DD62171B3B5DBECF3227CC43A17461이며 원래 NPC 파이프라인에 채택하지 않았다. 당시 기존 통합PBO는3BE0, 원본453D/자산0C6E다. analyze_horde_wallrate_repeated.py, analyze_horde_wallrate_part_commands.py, analyze_horde_interpolation_cycles.py의 기존 분석을 사용했다. 앞으로 원인 특정 전 보간/업데이트 대안을 추측으로 추가하지 않는다.

7. 원시 증거

- A/B/B/A 기존 보고서 (WALL_RATE_REPEATED_2026-10-28.md)와 같은 이름 JSON.
- 800개 보간 입력 분석 (INTERPOLATION_CYCLES_2026-10-28.json).
- runtime-20261028-013539 (runtime-20261028-013539)
- runtime-20261028-013820 (runtime-20261028-013820)
- runtime-20261028-014722 (runtime-20261028-014722)
- runtime-20261028-015001 (runtime-20261028-015001)
- runtime-20261028-003842 (runtime-20261028-003842)
- runtime-20261028-004145 (runtime-20261028-004145)

영상은 각 실행의 visible-clip-analysis.json의 video 경로를 따른다. JSON의 기존 videoSha256과 실제 파일을 대조했다. CPU/영상 원시는 visible-frame-clock.tsv, wall-rate-callback.tsv, native-call-probe/calls.jsonl이며 정확한 포함 파일/해시는 아래에 기록했다. 재현 조건은 고정run, AI/이동OFF, VSyncON, 동일그래픽seed, 같은15~70초전경창이다. native 훅 실행은 별도003842/004145이므로 A/B/B/A 영상과 동일 실행의 인과 증거로 합치지 않는다.

8. 잔여 문제

CPU 자세 변화 시각과 화면에서 같은 자세가 나타나는 시각의 지연·불연속, buffer 교체 전후 포즈 연속성을 읽기/관찰 중심으로 대조해야 한다. 현재 도구로 직접 얻지 못한 GPU/Present·compiled clip phase는 미확보로 유지한다. 안정성8/64 회귀와 성능4조건 대조는 별도 실험/보고로 관리한다. 새로운 보간·스케줄링 방식은 원인 근거 없이 적용하지 않는다.

원본 보고서 ID: 2026-10-28_023600_756451+0900_npc-animation-cpu-visible.md
원본 SHA-256: 4245a2d71ef769bb70e45e50c36cd93f9baf633bec7e83d0df65855fe94cc278
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

===== 추가 기술 근거 =====
[A1] 측정 절차: 아래 phase_pacing은 quarter-key/6본 FK proxy로 계산한 잔차다. 직접 clip 위상이 아니며 후보별 fit 오차의 영향을 받는다.

SOURCE: analyze_horde_wallrate_pair.py; lines 15-30; SHA256 1a89e939824b43dc6ada8eedbf13ba2094051f05d2b5a76b25cf36d6c9537e99
15: def phase_pacing(proxy):
16:     rows=proxy['hypotheses']['quaternion_standard']['matches'];pairs=[];all_pairs=[]
17:     for a,b in zip(rows,rows[1:]):
18:         dt=b['missionSeconds']-a['missionSeconds']
19:         step=((b['nearestSourceKey']-a['nearestSourceKey']+62.5)%125-62.5)/60
20:         if dt>0:all_pairs.append((dt,step))
21:         if b['frame']-a['frame']!=1 or not .005<=dt<=.04:continue
22:         if max(a['rmsPositionErrorMeters'],b['rmsPositionErrorMeters'])>.05:continue
23:         pairs.append((dt,step))
24:     mean_rate=sum(s for dt,s in all_pairs)/sum(dt for dt,s in all_pairs)
25:     residual=[(s/mean_rate-dt)*1000 for dt,s in pairs]
26:     return dict(pairs=len(pairs),fullWindowProxyMeanRate=mean_rate,
27:         selectedPairsMeanRate=sum(s for dt,s in pairs)/sum(dt for dt,s in pairs),
28:         rateNormalizedIncrementResidualRmsMs=math.sqrt(sum(v*v for v in residual)/len(residual)),
29:         rateNormalizedIncrementResidualAbsMs=quantiles([abs(v) for v in residual]),
30:         caveat='Quarter-key6-joint FK proxy with pose-fit gate; curvature/interpolation error differs by candidate. Not direct phase, GPU timing or an acceptance threshold.')

[A2] 직접 CPU/영상 시간 인과를 증명하는 데이터가 아니라, 같은 프레임의 변화량 비교 일부다. 상관계수는 지연/부드러움 수용 지표가 아니다.

runtime-20261028-013539/visible-pose-alignment.json; SHA256 0398024a07cb28cae5bb8d5420c91f11e4ed2a8d55e0950eafc200ab9334a774; 20초 구간 첫3개 admittedPairs 원문 JSON 항목:
[
  {
    "firstFrame": 1140,
    "lastFrame": 1141,
    "nativeFrameGap": 1,
    "videoSeconds": 20.016666666666666,
    "bodyMaskChangedPixels": 3434,
    "queriedRmsChangeMeters": {
      "-2": 0.01505750685981361,
      "-1": 0.015129335191386735,
      "0": 0.09237756696478498,
      "1": 0.03196119547461681,
      "2": 0.018068215347214957
    }
  },
  {
    "firstFrame": 1141,
    "lastFrame": 1142,
    "nativeFrameGap": 1,
    "videoSeconds": 20.033333333333335,
    "bodyMaskChangedPixels": 877,
    "queriedRmsChangeMeters": {
      "-2": 0.025202391470519878,
      "-1": 0.01505750685981361,
      "0": 0.015129335191386735,
      "1": 0.09237756696478498,
      "2": 0.03196119547461681
    }
  },
  {
    "firstFrame": 1142,
    "lastFrame": 1143,
    "nativeFrameGap": 1,
    "videoSeconds": 20.05,
    "bodyMaskChangedPixels": 863,
    "queriedRmsChangeMeters": {
      "-2": 0.05662771373629699,
      "-1": 0.025202391470519878,
      "0": 0.01505750685981361,
      "1": 0.015129335191386735,
      "2": 0.09237756696478498
    }
  }
]

runtime-20261028-013820/visible-pose-alignment.json; SHA256 e7c8e9160c5f8e1220ede6df595e8e72e0ec211ca9d4d9f9efa20adbd7872a21; 20초 구간 첫3개 admittedPairs 원문 JSON 항목:
[
  {
    "firstFrame": 1330,
    "lastFrame": 1331,
    "nativeFrameGap": 1,
    "videoSeconds": 20.033333333333335,
    "bodyMaskChangedPixels": 1834,
    "queriedRmsChangeMeters": {
      "-2": 0.05399914079563366,
      "-1": 0.015578024190078364,
      "0": 0.02361447960468322,
      "1": 0.13051274300938331,
      "2": 0.043999581759375836
    }
  },
  {
    "firstFrame": 1331,
    "lastFrame": 1332,
    "nativeFrameGap": 1,
    "videoSeconds": 20.05,
    "bodyMaskChangedPixels": 1373,
    "queriedRmsChangeMeters": {
      "-2": 0.12741279091336685,
      "-1": 0.05399914079563366,
      "0": 0.015578024190078364,
      "1": 0.02361447960468322,
      "2": 0.13051274300938331
    }
  },
  {
    "firstFrame": 1332,
    "lastFrame": 1333,
    "nativeFrameGap": 1,
    "videoSeconds": 20.066666666666666,
    "bodyMaskChangedPixels": 2387,
    "queriedRmsChangeMeters": {
      "-2": 0.04586241954658156,
      "-1": 0.12741279091336685,
      "0": 0.05399914079563366,
      "1": 0.015578024190078364,
      "2": 0.02361447960468322
    }
  }
]

runtime-20261028-014722/visible-pose-alignment.json; SHA256 9949428ccf8c47b84bfed967b0c26a0ae271362b005a80e8db60f0645bc87d48; 20초 구간 첫3개 admittedPairs 원문 JSON 항목:
[
  {
    "firstFrame": 1143,
    "lastFrame": 1144,
    "nativeFrameGap": 1,
    "videoSeconds": 20.016666666666666,
    "bodyMaskChangedPixels": 2851,
    "queriedRmsChangeMeters": {
      "-2": 0.0314045307585811,
      "-1": 0.15226808173032633,
      "0": 0.0602632866857315,
      "1": 0.02400905853700503,
      "2": 0.02415635503340682
    }
  },
  {
    "firstFrame": 1144,
    "lastFrame": 1145,
    "nativeFrameGap": 1,
    "videoSeconds": 20.033333333333335,
    "bodyMaskChangedPixels": 4756,
    "queriedRmsChangeMeters": {
      "-2": 0.020919889706528885,
      "-1": 0.0314045307585811,
      "0": 0.15226808173032633,
      "1": 0.0602632866857315,
      "2": 0.02400905853700503
    }
  },
  {
    "firstFrame": 1145,
    "lastFrame": 1146,
    "nativeFrameGap": 1,
    "videoSeconds": 20.05,
    "bodyMaskChangedPixels": 1362,
    "queriedRmsChangeMeters": {
      "-2": 0.039913021634549274,
      "-1": 0.020919889706528885,
      "0": 0.0314045307585811,
      "1": 0.15226808173032633,
      "2": 0.0602632866857315
    }
  }
]

runtime-20261028-015001/visible-pose-alignment.json; SHA256 223cf6fd127e4900e7296f34051a63fdb8b2e4c3b29bbd20261c24ca26be0d0f; 20초 구간 첫3개 admittedPairs 원문 JSON 항목:
[
  {
    "firstFrame": 1107,
    "lastFrame": 1108,
    "nativeFrameGap": 1,
    "videoSeconds": 20.016666666666666,
    "bodyMaskChangedPixels": 955,
    "queriedRmsChangeMeters": {
      "-2": 0.03997492997075372,
      "-1": 0.012989419534118281,
      "0": 0.01638163730624426,
      "1": 0.10113732780729376,
      "2": 0.03838278774832978
    }
  },
  {
    "firstFrame": 1108,
    "lastFrame": 1109,
    "nativeFrameGap": 1,
    "videoSeconds": 20.033333333333335,
    "bodyMaskChangedPixels": 757,
    "queriedRmsChangeMeters": {
      "-2": 0.09868667583485288,
      "-1": 0.03997492997075372,
      "0": 0.012989419534118281,
      "1": 0.01638163730624426,
      "2": 0.10113732780729376
    }
  },
  {
    "firstFrame": 1109,
    "lastFrame": 1110,
    "nativeFrameGap": 1,
    "videoSeconds": 20.066666666666666,
    "bodyMaskChangedPixels": 1882,
    "queriedRmsChangeMeters": {
      "-2": 0.03203545536328979,
      "-1": 0.09868667583485288,
      "0": 0.03997492997075372,
      "1": 0.012989419534118281,
      "2": 0.01638163730624426
    }
  }
]

[A3] 별도 native 훅 실행003842의 실제 보간 입력. 영상 실행과 동일시하지 말 것. 연속5개 이벤트의 필드만 투영했다.
SOURCE calls.jsonl SHA256 88754e5255447cd351080cbbd50adcd5f1d682e8d627fcdb5f1114cf1f2d61ab
[
  {
    "sourceLine": 2,
    "sequence": 2,
    "timeMs": 1793115584623,
    "numerator": 0.0429999977350235,
    "duration": 0.05000000074505806,
    "computedRatio": 0.8599999418854722,
    "savedInputDt": 0.035999998450279236,
    "primary": 0.05000000074505806,
    "accumulator": 0.0429999977350235
  },
  {
    "sourceLine": 2,
    "sequence": 3,
    "timeMs": 1793115584629,
    "numerator": 0.04899999871850014,
    "duration": 0.05000000074505806,
    "computedRatio": 0.9799999597668654,
    "savedInputDt": 0.006000000052154064,
    "primary": 0.05000000074505806,
    "accumulator": 0.04899999871850014
  },
  {
    "sourceLine": 2,
    "sequence": 4,
    "timeMs": 1793115584660,
    "numerator": 0,
    "duration": 0.05000000074505806,
    "computedRatio": 0,
    "savedInputDt": 0.03099999949336052,
    "primary": 0.05000000074505806,
    "accumulator": 0
  },
  {
    "sourceLine": 2,
    "sequence": 5,
    "timeMs": 1793115584667,
    "numerator": 0.00800000037997961,
    "duration": 0.05000000074505806,
    "computedRatio": 0.16000000521540633,
    "savedInputDt": 0.00800000037997961,
    "primary": 0.05000000074505806,
    "accumulator": 0.00800000037997961
  },
  {
    "sourceLine": 2,
    "sequence": 6,
    "timeMs": 1793115584700,
    "numerator": 0.04000000283122063,
    "duration": 0.05000000074505806,
    "computedRatio": 0.800000044703483,
    "savedInputDt": 0.03200000151991844,
    "primary": 0.05000000074505806,
    "accumulator": 0.04000000283122063
  }
]

[A4] 누락: 원본 영상 픽셀/전체 마스크, GPU/Present 시각, 직접 compiled clip phase, buffer 교체 전후 실제 렌더 소비 포즈. 이 텍스트만으로 시각적 끊김의 체감이나 정확한 원인을 확정할 수 없다.

===== 검토 요청 =====
1. 제시한 증거로 확정할 수 있는 결론과 과도한 결론을 나눠라.
2. 반례·누락된 상태/호출 관계·측정 편향을 찾아라.
3. 원인을 구분할 다음 최소 실험과 필요한 추가 자료를 제안하라.
4. 코드 제안은 가설로 표시하고 실제 수정/수용 완료로 표현하지 말라.
