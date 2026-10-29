# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T224237-entry-preparation-resume-20261030

# 711C ON 회귀 검토 패킷

## 1. 작업 목표

미뤄졌던 711C 준비 접촉 ON의 실제 8→64개체 기능 회귀를 수행하고, 정상 낙하와 미해결 낙하·등반 계약 실패를 구분한다. 애니메이션 실제 표시와 성능 기여도는 별도 실험이다. B8FB는 이미 8/64 시험을 거쳤으므로 과거 next의 미실행 문구를 현재 사실로 반복하지 않는다.

## 2. 실제 수행

711C 보관본·서명·기존11개 script 동일성·시험 씬·기존14개 클립 검증 기록의 의존성을 재확인했다. 같은 씬·계측·그래픽·300초에서8 ON, 통과 확인 뒤64 ON을 순차 실행했다. 두 실행 정상 종료 후 기존 상태·고지대·접촉·입구·복구·시계 분석기를 실행했다. computer-use로 실제 장면도 한 시점씩 확인했다. 연속 영상/표시 부드러움 검증은 아니다. 원시 실험/운영 서버는 보존하고 D61D 서명본을 복원했다.

## 3. 측정 결과

|항목|8 ON 072332|64 ON 073005|
|---|---:|---:|
|최종 도착|8/8|63/64|
|HIGH_HOLD 안 고지대 누락|0|0|
|낙하 후 착지·FOLLOW 복귀|10|86|
|CLIMB_EXIT_VALIDATION|0|1 (50번)|
|착지 후 owner 계약 위반|0|0|
|동일 호출 물리 전환 연결|전체 일치|전체 일치|
|준비 변위 변경/수용 표본|11/11|109/103|
|전경 미션 p50/p95/p99|20/32/34ms|27/68/88ms|

한 번씩의 기능 시험이다. frame은 미션 갱신 간격이며 GPU Present/본 표시율/self CPU 비용이 아니다. 과거8 OFF와 설정상 단일요인 비교는 같지만 외부 부하·날짜가 달라35→32ms를 정책의 성능 개선으로 귀속하지 않는다. 반복 비교 미완료다.

12번은107.15초 세 번째 낙하에 진입해 마지막299.753초에도 FALL이며 LAND 전환이 없었다.50번은52.56초 CLIMB_EXIT_VALIDATION 후 복귀했다. 최종 도착이 중간 실패를 지우지 않는다.

## 4. 원인 분석

확인:12번의 세 번째 낙하 후 접촉 실패375표본 중203개는 낮은 컨테이너 대상 joint0 검사다. 나머지에는 foot lift limit와 발 관절 거부가 섞인다. 마지막 위치는 고정됐고 FALL_NATIVE_JOINT_WORLD_0였다. 실제 경로는 previous-contact Fall wrapper → landing-detail Fall → Accept → SolveContacts다. Accept 거부 시 위치를 커밋하지 않고 탈출 시도 후 반환한다.

가설: 컨테이너와 보존된 관절 접촉/포즈 때문에 낙하 후보가 계속 거부될 수 있다. 근본 원인은 미확정이다. 로그는 표본이며 초반 모든 호출을 덮지 않는다. landing observer표본0도 해당 분기가 전혀 실행되지 않았다는 증명이 아니다. 실제 이전 접촉·본·바닥 레이 동결 재생이 필요하다.

50번은 endpoint error약0.000038m, exit error약0.017833m인데 FOOT_Z_SUPPORT로 거부됐다. root거리만 완화해서는 안 된다. 마지막 발 지지와 lane2입구·출구·접촉을 별도로 재현해야 한다. 단일 실행으로 ON에 원인을 귀속하지 않는다.

## 5. 검증 수준

8 ON:해당 씬 단일 기능 회귀 통과.64 ON:진단 성공·기능 회귀 실패.준비 분기 실제 적용은 표본에서 확인.반복 검증 완료·최종 수용 아님.애니메이션 표시 개선 미측정.성능 기여도 미측정.20ms p95 목표 미달.전체 골 미완료.

## 6. 변경 사항

게임 코드·그래프·PBO 새 수정/빌드 없음. 기존 서명711C를 시험 중 활성화하고 두 정상 종료 후 서명D61D로 복원·검증했다. 의존PBO3개 서명 검증 성공, 복원본 서명 검증 성공.개인 키 공개/배포 없음.새 분석 결과와 실패 선정 자료만 저장했다.

## 7. 원시 증거

runtime-20261030-072332 및 runtime-20261030-073005의 launch/state-contract/native-ground/contact-failures/post-touchdown/entry-preparation/frame-clock과 분석 JSON을 해시 연결한다. 로컬 entry-preparation-resume-20261030에 preflight, 활성화·복원 증거,8단일요인 비교,actor12-50선정 JSON이 있다. 공개본은 집계·선정 기록이며 전체 raw/로컬 경로/키/게임 자산을 공개하지 않는다.

## 8. 잔여 문제

다음 기능 실험:12번 세 번째 낙하의 실제 이전 접촉과 포즈를 동결 재생하고,50번 마지막 발 지지/lane2끝점은 별도 재현한다. 지지 검사·정상 낙하를 끄거나 위치를 강제 보정하지 않는다. 재현 후 최소 수정과 새8→64·반복 OFF/ON 회귀로 확대한다.

애니메이션은 CPU 포즈/그래프 시계/실제 표시 불일치로 별도 유지한다. 새 지원 제어 소비 지점 확보 전 추측 보간·배속을 적용하지 않는다.64비용은 정지1/64애니메이션 및64 static/animated active 조건으로 별도 측정한다. 현재p95를 AI/충돌/렌더/GC로 나눌 수 없다. 이번은 기능 묶음 종료이며 전체 작업 종료가 아니다.

선정 기준:12번 미해결 낙하와50번 유일한 CLIMB_EXIT_VALIDATION.정상 복구86건 보존.전체 로그가 아니다.


선정 함수: DGSubsetZLandingBackoffDetail.c, 시작행 70, 원문 SHA256 11c1f45fc74c8fedfc11779b0e1f69bef8371995e932539ae6be092b9df97168
선정 이유: 실제 낙하와 위치 커밋/거부 경로. 전체 상속/엔진 구현이 아니다.
```cpp
override void Fall(float dt)
    {
        if(!LDOn()){super.Fall(dt);return;}
        if(!Body.FrameFallPhysicsActive())
        {
            if(Recovering && Body.Request!=8)return;
            if(!Recovering && Body.Request!=6 && Body.Request!=8)return;
        }
        bool escaped=EscapeStep(dt);
        FallVelocity=Math.Max(-15,FallVelocity - 9.81*dt);
        vector next=Body.Agent.Position+Vector(0,FallVelocity*dt,0);
        if(PressureFallActive)next+=PressureFallVelocity*dt;
        Object hit;vector point,normal;float fraction;bool landed=false;
        if(DayZPhysics.RayCastBullet(Body.Agent.Position+"0 0.12 0",next+"0 -0.1 0",DGBGroundAgent.Floors(),Body.Leader,hit,point,normal,fraction))
        {
            if(normal[1]>=0.65 && point[1]<=Body.Agent.Position[1]+0.05 && point[1]>=next[1] - 0.1)
            {
                vector support;bool observedLandingValid=LandingSpace(point,support,!Body.Agent.ManagedPeerContacts);
                LDObserveLanding(point,support,observedLandingValid,!Body.Agent.ManagedPeerContacts);
                if(observedLandingValid){next=support;landed=true;}
                else{if(!escaped){if(PressureFallActive){if(!PressureSlide(dt))AirEscapeTurn(dt);}else EscapeStep(dt);}Body.Reason="LANDING_SPACE_BLOCKED";return;}
            }
        }
        if(!Accept(next,dt)){if(!escaped){if(PressureFallActive){if(!PressureSlide(dt))AirEscapeTurn(dt);}else EscapeStep(dt);}Body.Reason="FALL_"+Body.Reason;return;}
        Body.Reason="NATIVE_FALL";
        if(landed)
        {
            PressureFallActive=false;PressureFallVelocity=vector.Zero;
            PressureEscapeValid=false;NextPressureEscapePlan=0;
            Landings++;WantLanding=true;LandingDelay=0;
            if(FallPresentationRequest==8 && FallStart-Body.Agent.Position[1]>0.75){LandingDelay=0.7;}
            Body.NextRequest=0;
            EgressGoal=Body.Agent.Position;
            if(Active)
            {
                EgressGoal=Body.Agent.Position+DGBNativeGroundBody.Rotate("0 0 -1.2",Active.Heading);
                if(Recovering && Active.Action=="CLIMB")EgressGoal=Body.Agent.Position - DGBNativeGroundBody.Rotate("0 0 -1.2",Active.Heading);
            }
            SetPhase("LAND");
        }
    }
```


선정 함수: DGBNativeTraversal.c, 시작행 725, 원문 SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872
선정 이유: 실제 낙하와 위치 커밋/거부 경로. 전체 상속/엔진 구현이 아니다.
```cpp
bool Accept(vector next,float dt)
    {
        MotionContactBlocked=false;
        if(Body.Director.Crowd)
        {
            string rejection;
            if(!Body.Director.Crowd.CoreClear(Body,Body.Agent.Position,next,rejection))
            {
                Body.Reason="TRAVERSAL_"+rejection;
                if(Body.Director.OpenTraversal && Phase=="MOTION" && (rejection.IndexOf("PEER_CORE:")==0 || rejection=="PLAYER_CORE"))
                {
                    // A rejected proposed step did not move this actor off its
                    // support. Contact is not evidence of a physical fall.
                    MotionContactBlocked=true;MotionContactBlocks++;LastEntityContact=rejection;
                }
                return false;
            }
        }
        float lift;ref array<vector> points={};
        if(!Body.SolveContacts(next,Body.Agent.Heading,lift,points))
        {Body.PoseBlocks++;return false;}
        float travel=vector.Distance(Body.Agent.Position,next);
        Body.Agent.Position=next;Body.Agent.Moved+=travel;Body.Agent.MeasuredSpeed=travel/Math.Max(dt,0.001);
        Body.RootAccepted+=travel;Body.Lift=lift;Body.Contacts.Copy(points);
        return true;
    }
```

선정 기록(전체 로그 아님):
```json
{
  "sourceRunId": "runtime-20261030-073005",
  "actor12Last": {
    "seconds": "299.753",
    "id": "12",
    "position": "<4511.270020, 339.357056, 10237.890625>",
    "reason": "FALL_NATIVE_JOINT_WORLD_0",
    "traversalPhase": "FALL",
    "pressureEscapeDiagnostic": "SEPARATION_DISTANCE_LIMIT",
    "pressureEscapeLastStepReject": "NEW_WORLD_CONTACT",
    "pressureEscapeRejectFixture": "StaticObj_Container_1C"
  },
  "actor12Counts": {
    "groundSamples": 375,
    "contactFailureSamples": 375,
    "contactFailureJoints": {
      "3": 115,
      "0": 203,
      "2": 57
    },
    "landingObserverSamples": 0
  },
  "actor50Transitions": [
    {
      "sequence": "798",
      "seconds": "52.56",
      "id": "50",
      "kind": "FALL_REQUEST",
      "previous": "MOTION",
      "next": "FALL",
      "failure": "CLIMB_EXIT_VALIDATION",
      "detail": "RECOVERY_FALL",
      "motionElapsedBeforeTransition": "2.15",
      "endpointError": "3.8071e-05",
      "exitError": "0.0178332",
      "link": "low_to_high_lane_2"
    },
    {
      "sequence": "799",
      "seconds": "52.56",
      "id": "50",
      "kind": "TRANSITION",
      "previous": "MOTION",
      "next": "FALL",
      "failure": "CLIMB_EXIT_VALIDATION",
      "detail": "FOOT_Z_SUPPORT",
      "motionElapsedBeforeTransition": "2.15",
      "endpointError": "3.8071e-05",
      "exitError": "0.0178332",
      "link": "low_to_high_lane_2"
    }
  ],
  "limitations": [
    "Contact failure logging is sampled; absence is not proof that a branch never ran.",
    "No new replay, physics queries or pose/control changes; same-run correlation is not root-cause proof."
  ]
}
```

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
