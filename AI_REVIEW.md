# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T112003-captured-contact-replay-20261029

실제 낙하 입력 재현과 접평면 후보 — 2/4 부분 통과, 전체 미수용

작성: 2026-10-29T20:19:06.102348+09:00

1. 작업 목표

실제 낙하 기록의 이전 관절 접촉점·현재 포즈·dt·속도로 착지 거부를 재현한 뒤, 접촉면에 남은 이동을 투영하는 제한된 후보 하나를 시험한다. 기존 정상 낙하·복구 및 배포 PBO는 보존한다.

2. 실제 수행

원래 SolveContactsInternal과 고정 입력용 함수가 본 getter 하나의 대체 외 동일함을 확인했다. 실제4건 각각에 직접 LandingSpace/SolveContacts와 원래 Fall 분기를 실행했다. 이어 동일 입력·동일 대조군에 단일 후보를 추가했다. 남은 root 이동(1-hitFraction)을 실제 관절 충돌 법선의 접평면에 투영하고 수평 이동을 min(0.06m,1.2m/s*실제dt)로 제한했다. 상승은 허용하지 않았다. 기존 PressureEscapeStepClear의 전체 공중 검사와 원래 Accept/SolveContacts를 모두 유지했다. 성공해도 FALL 상태를 유지했다.

첫2개 후보 미션은 벡터 축소식 부근의 Enforce 내부 컴파일 오류로 실패했다. 실패 미션·컴파일 결과·로그를 보존했다. 명시적인 float 중간 변수 및 vector 대입식으로 변경한3번째가 통과했고, 같은 소스의 별도 반복 미션도 통과했다. 이 변경은 시험 표현식 수정이며 런타임 알고리즘 튜닝이 아니다.

3. 측정 결과

기준 실제4건×2모드=8/8에서 발판 허용 true, 관절0 월드 swept contact 거부, FALL 유지·root/기존 접촉점 불변을 재현했다. 원래 충돌 법선/비율과의 차이는 각각2e-6 이하로 검증했다.

후보 실행과 반복 실행 각각 대조군8/8 모든 출력 필드가 기준과 동일했다. 후보4건 중 id4/seq226 및 id32/seq152는 안전한 공중 이동이 허용됐다. 수평 이동은 약0.03120m/0.01963m, 전체 이동은0.09413m/0.07550m였다. id23/seq197 및 id43/seq284는 NEW_WORLD_CONTACT에서 거부돼 root 변화0이었다. 4건 모두 FALL 유지, 착지 완료를 인위적으로 만들지 않았다. 후보 결과 행은 두 실행에서 동일했다.

이번에는 실제8/64 게임 회귀·시간적 화면 관찰·p50/p95/p99를 실행/측정하지 않았다. 최신 실제64 기록의 p95 149ms는 이전 실험 수치이며 이번 후보 성능으로 재사용하지 않는다.20ms는 목표다.

4. 원인 분석

사실: 발판 허용과 물리적 LAND 사이에서 원래 골반 관절 sweep이 거부할 수 있다. 실제 입력4건에서 이 분기가 재현됐다. 단일 접평면 후보는2건만 모든 검사를 통과하고2건은 새로운 접촉으로 거부된다.

추정: 다중 접촉면/발 위치와 경사 모서리가 남은2건의 이동을 제한할 수 있다. 이번 NEW_WORLD_CONTACT는 구체적인 probe 인덱스·법선까지 출력하지 않아 어느 면인지 확정하지 않는다.

반증/제한: 단일 법선 투영만으로4건 모두를 복구할 수 있다는 가설은 이 조건에서 성립하지 않는다.2건의 한 스텝 통과는 최종 착지나 지속 추적의 증거가 아니다. CPU 포즈·렌더 시간 불일치의 원인도 이번 실험으로 확정하지 않는다.

5. 검증 수준

진단 성공: 실제 입력 기준 재현4건/8검사. 반복 검증 완료: 고정 입력 후보와 대조군 결과의2회 일치. 전체8→64 기능 회귀·시각 품질·성능·최종 수용은 미실행/미수용이다. 후보를 배포하거나 기능 해결로 승격하지 않았다.

6. 변경 사항

시험 도구: stage/analyze_horde_captured_contact_inputs.py, stage/analyze_horde_captured_contact_slide.py 및 고유 validation 허용·marker를 추가한 compile_days_gone_native_runtime_probe.ps1. 생성 미션은 격리 lab 전용이다. 배포 integration D61DE123 및 base453D6A9E/render0C6E1855 PBO는 변경하지 않았다. 새 PBO 빌드/서명은 없고 각 엔진 실행 전 기존 integration/render 서명 검증이 통과했다. 비밀키는 배포하지 않았다.

7. 원시 증거

실제 원본은 runtime-20261029-191331 seq226 및 runtime-20261029-193252 seq152/197/284다. 고정 입력/이전 Contacts/포즈/압력속도/dt를 사용하고 peer와 live graph는 없다. PressureSlide/AirEscapeTurn 후속 복구를 억제했으므로 전체 궤적 재연이 아니다. baseline/후보/반복 실행 ID, TSV·JSON·실패 로그·함수 source 및 SHA-256은 아래 무결성 목록과 검토 패킷에 연결한다. child native bone 진단은 생성 leader를 읽으며 solver의 고정 포즈와 동일하지 않다.

8. 잔여 문제

다음은 NEW_WORLD_CONTACT의 probe 인덱스·시작/끝점·법선·비율을 읽기 전용으로 추가 수집해 남은2건을 구분한다. 이후 연속 낙하 입력/포즈 변화에서도 실제 LAND까지 이어지는지 검증해야 한다. 그 전에는 live 수정과8→64 회귀로 승격하지 않는다. 고지대 미도달·미해결 혼잡·등반 종료 계약, CPU/실제 표시 시각 차이, 통제64 비용 분석은 계속 별도 미완료로 유지한다.

원본 보고서 ID: 2026-10-29_201906_102348+0900_npc-captured-contact-replay.md
원본 SHA-256: 8207b7dfae4bc110f18e9b4858a6b035420f75009bb3e50cddba1c8372c63f53
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

{
  "baselineResults": [
    {
      "case": 0,
      "mode": 0,
      "sourceRunId": "runtime-20261029-191331",
      "sourceSequence": 226,
      "id": 4,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 0,
      "mode": 1,
      "sourceRunId": "runtime-20261029-191331",
      "sourceSequence": 226,
      "id": 4,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 1,
      "mode": 0,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 152,
      "id": 32,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 1,
      "mode": 1,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 152,
      "id": 32,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 2,
      "mode": 0,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 197,
      "id": 23,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 2,
      "mode": 1,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 197,
      "id": 23,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 3,
      "mode": 0,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 284,
      "id": 43,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    },
    {
      "case": 3,
      "mode": 1,
      "sourceRunId": "runtime-20261029-193252",
      "sourceSequence": 284,
      "id": 43,
      "landingSpaceAccepted": true,
      "fullContactRejected": true,
      "finalPhase": "FALL",
      "normalDifference": 0.0,
      "fractionDifference": 0.0,
      "supportDifferenceMetres": 0.0,
      "rootMoved": false,
      "cachedHipsChanged": false
    }
  ],
  "candidateResults": [
    {
      "case": "0",
      "sourceSequence": "226",
      "id": "4",
      "mode": "2",
      "attempted": "true",
      "gate": "ACCEPTED_STILL_FALL",
      "accepted": "true",
      "before": "<4511.482422, 339.321655, 10243.338867>",
      "proposed": "<4511.511719, 339.232849, 10243.349609>",
      "after": "<4511.511719, 339.232849, 10243.349609>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NONDEEPENING_CLEAR",
      "horizontalMetres": 0.031204242868733183,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.09412869067939313
    },
    {
      "case": "1",
      "sourceSequence": "152",
      "id": "32",
      "mode": "2",
      "attempted": "true",
      "gate": "ACCEPTED_STILL_FALL",
      "accepted": "true",
      "before": "<4511.291016, 339.454620, 10236.513672>",
      "proposed": "<4511.292969, 339.381714, 10236.494141>",
      "after": "<4511.292969, 339.381714, 10236.494141>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NONDEEPENING_CLEAR",
      "horizontalMetres": 0.019628402125403094,
      "lateralBoundMetres": 0.03358152,
      "movedMetres": 0.07550204636958478
    },
    {
      "case": "2",
      "sourceSequence": "197",
      "id": "23",
      "mode": "2",
      "attempted": "true",
      "gate": "AIR_FULL_SWEEP",
      "accepted": "false",
      "before": "<4511.441406, 339.370117, 10243.416016>",
      "proposed": "<4511.457031, 339.293152, 10243.427734>",
      "after": "<4511.441406, 339.370117, 10243.416016>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NEW_WORLD_CONTACT",
      "horizontalMetres": 0.019530800010155225,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.0
    },
    {
      "case": "3",
      "sourceSequence": "284",
      "id": "43",
      "mode": "2",
      "attempted": "true",
      "gate": "AIR_FULL_SWEEP",
      "accepted": "false",
      "before": "<4511.488281, 339.377655, 10236.717773>",
      "proposed": "<4511.520020, 339.294220, 10236.717773>",
      "after": "<4511.488281, 339.377655, 10236.717773>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NEW_WORLD_CONTACT",
      "horizontalMetres": 0.03173900000001595,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.0
    }
  ],
  "repeatResults": [
    {
      "case": "0",
      "sourceSequence": "226",
      "id": "4",
      "mode": "2",
      "attempted": "true",
      "gate": "ACCEPTED_STILL_FALL",
      "accepted": "true",
      "before": "<4511.482422, 339.321655, 10243.338867>",
      "proposed": "<4511.511719, 339.232849, 10243.349609>",
      "after": "<4511.511719, 339.232849, 10243.349609>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NONDEEPENING_CLEAR",
      "horizontalMetres": 0.031204242868733183,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.09412869067939313
    },
    {
      "case": "1",
      "sourceSequence": "152",
      "id": "32",
      "mode": "2",
      "attempted": "true",
      "gate": "ACCEPTED_STILL_FALL",
      "accepted": "true",
      "before": "<4511.291016, 339.454620, 10236.513672>",
      "proposed": "<4511.292969, 339.381714, 10236.494141>",
      "after": "<4511.292969, 339.381714, 10236.494141>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NONDEEPENING_CLEAR",
      "horizontalMetres": 0.019628402125403094,
      "lateralBoundMetres": 0.03358152,
      "movedMetres": 0.07550204636958478
    },
    {
      "case": "2",
      "sourceSequence": "197",
      "id": "23",
      "mode": "2",
      "attempted": "true",
      "gate": "AIR_FULL_SWEEP",
      "accepted": "false",
      "before": "<4511.441406, 339.370117, 10243.416016>",
      "proposed": "<4511.457031, 339.293152, 10243.427734>",
      "after": "<4511.441406, 339.370117, 10243.416016>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NEW_WORLD_CONTACT",
      "horizontalMetres": 0.019530800010155225,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.0
    },
    {
      "case": "3",
      "sourceSequence": "284",
      "id": "43",
      "mode": "2",
      "attempted": "true",
      "gate": "AIR_FULL_SWEEP",
      "accepted": "false",
      "before": "<4511.488281, 339.377655, 10236.717773>",
      "proposed": "<4511.520020, 339.294220, 10236.717773>",
      "after": "<4511.488281, 339.377655, 10236.717773>",
      "phase": "FALL",
      "reason": "FALL_NATIVE_JOINT_WORLD_0",
      "airGate": "NEW_WORLD_CONTACT",
      "horizontalMetres": 0.03173900000001595,
      "lateralBoundMetres": 0.06,
      "movedMetres": 0.0
    }
  ],
  "runs": [
    {
      "name": "controlled-captured-contact-inputs",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-5136c012bef6456a990ecf6099f9f5b1"
    },
    {
      "name": "controlled-captured-contact-slide",
      "enginePassed": false,
      "nativeCrash": true,
      "profile": "compile-a18a5385ea5b45dfa749390b5d64cd08"
    },
    {
      "name": "controlled-captured-contact-slide-typed",
      "enginePassed": false,
      "nativeCrash": true,
      "profile": "compile-482577a077c143de898965937ca58e63"
    },
    {
      "name": "controlled-captured-contact-slide-scalar",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-8efebb82200f470a8421ce05de2403cb"
    },
    {
      "name": "controlled-captured-contact-slide-scalar-repeat",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-87fc0f56678841fab677fa5a6efae892"
    }
  ],
  "selection": "All4capturedcases;notwhole64logsorlivetrajectory."
}
Selected generated Enforce candidate block;logging boilerplate omitted:
  bool candidateAttempt=false;bool candidateAccepted=false;string candidateGate="NOT_REQUESTED";
  vector proposed=body.Agent.Position;vector beforeCandidate=body.Agent.Position;
  if(mode==2 && traversal.Phase=="FALL" && body.Reason=="FALL_NATIVE_JOINT_WORLD_0")
  {
   candidateAttempt=true;candidateGate="NORMAL";
   vector contactNormal=body.HitNormal;float contactFraction=body.HitFraction;
   if(contactNormal.Length()>0.99 && contactNormal.Length()<1.01 && contactFraction>=0 && contactFraction<=1)
   {
    vector remaining=(supportTable[test] - body.Agent.Position)*(1 - contactFraction);
    vector slide=remaining - contactNormal*vector.Dot(remaining,contactNormal);
    candidateGate="DOWNWARD";
    vector recordedSupport=supportTable[test];vector currentRoot=body.Agent.Position;
    if(slide[1]<=0 && slide[1]>=recordedSupport[1] - currentRoot[1])
    {
     float horizontal=Math.Sqrt(slide[0]*slide[0]+slide[2]*slide[2]);
     float recordedDt=dtTable[test];float limit=Math.Min(0.06,1.2*recordedDt);
     if(horizontal>limit){float scale=limit/horizontal;slide=slide*scale;}
     proposed=body.Agent.Position+slide;
     float proposedLift;ref array<vector> proposedContacts={};candidateGate="AIR_FULL_SWEEP";
     if(traversal.PressureEscapeStepClear(proposed,proposedLift,proposedContacts))
     {
      candidateGate="ACCEPT_FULL_CONTACT";
      candidateAccepted=traversal.Accept(proposed,dtTable[test]);
      if(candidateAccepted)candidateGate="ACCEPTED_STILL_FALL";
     }
    }
   }
  }

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
