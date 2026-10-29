# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T113929-contact-episode-review-20261029

낙하 거부의 episode 재분류 — 실제4/4 복구, 접평면 후보 배포 보류

작성: 2026-10-29T20:39:08.181028+09:00

1. 작업 목표

이전 단일 접평면 후보에서 남은2건의 거부 프로브를 확인하고, 두 접촉면 후보와 연속 낙하를 검사한다. 이어 고정 입력의 거부를 실제 실행의 미종결 복구와 혼동하지 않도록 원래 개체 episode 종료까지 대조한다. 정상 낙하·기존 복구·배포 동작은 보존한다.

2. 실제 수행

새 물리 query 없이 기존 PressureEscapeReject 필드의 probe/radius/시작끝점/법선/비율을 읽었다. 활성 gate는 base만이 아니라 D61D의 terrain-aware override였고, post-touchdown override도 존재한다. source/packed 바이트와 TC=true/PT=false 설정을 확인한 뒤 동일 gate를 유지했다. 로그22필드를 한 식으로 연결한 첫 미션은 Formula too complex로 실패했다. 원본 실패 기록을 보존하고5필드씩 연결한 고유 미션에서 통과했다.

첫 법선은 관절0, 둘째는 새로 거부한 몸통 프로브 법선이다. 같은 하강/수평 제한 아래 두 평면 교선 방향의 후보를1회 추가했다. 모든 기존 gate와 Accept/SolveContacts를 유지했고 후보는 LAND를 설정하지 않는다. 이어 실제 초기 dt/압력속도/포즈를 사용해 최대240회 원래 Fall을 호출하는 기준/후보 각4궤적을 실행하고 같은 소스로 반복했다. 포즈와 native clock은 고정, PressureSlide/AirEscapeTurn은 기준과 후보 모두 억제된 원인 분리 실험이다.

이 억제 조건을 실제 코드의 실패로 오해하지 않도록 실제4개체의 후속 LD→FALL/LAND→EGRESS/FOLLOW 기록을 같은 id/generation으로 추가 감사했다.

3. 측정 결과

추가 관측 전후 기존12개 결과 행은 바이트 동일했다. 남은 두 거부는 모두 root+0.9m 몸통 probe1·반경0.23m·컨테이너0·hit fraction0이었다. 시작/끝 보수적 box gap은 양수이나 native법선과 이동 내적은 id23 -0.0117973, id43 -0.0139123으로 안쪽 방향이었다. 두 평면 후보에서 원래12개 대조 행은 그대로였고4건 중3건이 안전한 한 스텝을 통과했다. 마지막 id43은 같은 probe에서 다른 새 법선으로 다시 거부됐다.

고정 포즈·후속 복구 억제 연속 실험은2회 동일했다. 기준0/4 physical LAND, 후보3/4 physical LAND였다. id4는2스텝/0.100초, id32는2스텝/0.0559692초, id23은11스텝/0.550초였다. id43은240스텝/12초 정지했다. LAND는 helper가 아닌 원래 Fall의 물리 전환이며 각 실행3개 상태 전환과 대조했다.

그러나 실제 실행의 같은4개체는 모두 기존 코드로 복구했다:

|개체|거부 질의→실제LAND 초|거부 질의→FOLLOW 초|실제LAND와 맞는 후속 질의|
|---:|---:|---:|---:|
|4|0.000|12.658|227|
|32|0.130|8.547|153|
|23|0.000|8.767|198|
|43|0.000|10.372|285|

이들은 최초 거부 anchor와 다른 위치의 후속 착지 질의가 실제 LAND에 대응했다.3건은같은기록시각,1건은0.130초후였다. 따라서 선택4episode의 미종결 낙하는0이다. 이번에는 실제8/64 새 회귀·화면 시간 측정·p50/p95/p99 측정이 없다. 기존64 고지대 미도달25/27과p95149ms는 이전 실행 수치로 남기고 후보 성과로 재사용하지 않는다.20ms는 목표다.

4. 원인 분석

사실: 관절 sweep 거부는 실제지만 같은 fall episode의 최종 실패는 아니었다. 원래 Fall의 후속 옆 이동 복구를 억제한 counterfactual이 기준0/4를 만든다. 실제 후속 LD에는 FALL_CONTACT_AIR_SEPARATION이 기록됐고4/4가 LAND와 FOLLOW로 이어졌다. 위치가 다른 최초 질의의 physicalMatches=[]를 전체 episode 미종결로 해석해서는 안 된다.

추정: 작은 동작의 native 법선 변화는 모서리의 여러 접촉면 또는 native sweep 수치 특성의 영향일 수 있다. 이번 자료만으로 두 요인의 기여를 나누지 않는다. 이후 우선순위는 실제 고지대 미도달/긴 LAND→FOLLOW 구간의 계약·시간 분석이며 이4건을 억지로4/4 고정 포즈 통과시키는 일이 아니다.

반증: 4개 anchor 거부가4개 실제 미종결 낙하라는 해석은 반증됐다. 고정 입력 후보3/4 LAND가 실제 기존 파이프라인4/4복구보다 개선됐다는 근거도 없다. 후보는 배포하지 않는다. 이전 보고서는 원문 보존하고 이 보고서에서 해석 범위를 명확히 보완한다.

5. 검증 수준

진단 성공: 거부 probe 귀속·실제4episode 종료 감사. 반복 검증 완료: 후속 복구를 억제한 고정 포즈8궤적 결과의2회 일치. 후보의 실제8→64 회귀·시각 품질·성능·최종 수용은 미실행/미수용이다. 선택4episode 복구를 다른 개체·전체 고지대 계약의 해결로 확대하지 않는다.

6. 변경 사항

gate detail/two-plane/continuous stage·analyze 도구와 episode audit 도구, 고유 validation 허용 및 marker만 추가했다. 새 생성 미션은 격리 lab 전용이다. D61DE123 PBO는 해시 동일하며 이동·애니메이션 코드 수정/새 빌드/새 서명은 없다. 각 엔진 실행 전 기존 integration/render 서명 검증은 통과했다. 비밀키는 공개/배포하지 않았다.

7. 원시 증거

고유 engine profile·실패/성공 compile JSON·gate TSV·continuous TSV·실제 원본 LD/상태 TSV·도구 source는 아래 무결성 목록에 있다. 실제 원본은 runtime-20261029-191331 id4/seq226 및 runtime-20261029-193252 id32/seq152,id23/seq197,id43/seq284다. 선택 기준은 이전4개 실제 관절0 거부 전부다. 해당 episode를 끝까지 추적했지만 exact per-Fall-call serial은 없으므로 동일 director시각을 고유 호출 동일성으로 주장하지 않는다.

8. 잔여 문제

접평면 후보는 보류하고 기존 복구를 유지한다. 다음은 실제64 고지대 미도달25/27명을 pressure fall 후 정상 복구, 진입/등반 지연, LAND/EGRESS 지연, 미종결 계약으로 구분한다. 특히 실제 선택4건의 LAND→FOLLOW 약8~13초 구간이 안전한 포즈 정리인지 불필요한 지연인지 근거를 확인한다. CPU/native callback clock과 실제 화면의 시간적 차이 및64비용의 통제 실험은 별도 미완료다. 다른 승인 작업의 데스크톱 사용 요청을 존중했고 화면 입력/캡처·공유 런타임/타 프로세스는 조작하지 않았다.

원본 보고서 ID: 2026-10-29_203908_181028+0900_npc-contact-episode-review.md
원본 SHA-256: 46ecf7411c55c7b6c5b403f3726e1d6a5fe21665feddf911686c2dd50a6376b1
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

{
  "validations": [
    {
      "name": "controlled-captured-contact-slide-gates",
      "enginePassed": false,
      "nativeCrash": true,
      "profile": "compile-02402bf71620493785c9bdcb8941507d"
    },
    {
      "name": "controlled-captured-contact-slide-gates-chunks",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-0f0a5e91dc644108b6834d497d30b6c6"
    },
    {
      "name": "controlled-captured-contact-two-plane",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-a9c465e252ee45638e73c6bd54b9f533"
    },
    {
      "name": "controlled-captured-contact-continuous",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-581f6a24009f42108998971b70e8eff4"
    },
    {
      "name": "controlled-captured-contact-continuous-repeat",
      "enginePassed": true,
      "nativeCrash": false,
      "profile": "compile-8453e90610504f69a0407abe51cf2994"
    }
  ],
  "gateRejections": [
    {
      "case": "2",
      "sourceSequence": "197",
      "id": "23",
      "mode": "2",
      "attempted": "true",
      "accepted": "false",
      "gate": "NEW_WORLD_CONTACT",
      "probe": "1",
      "fixture": "StaticObj_Container_1C",
      "fixtureIndex": "0",
      "fixturePosition": "<4510.000000, 338.823303, 10240.000000>",
      "oldLift": "0",
      "lift": "0",
      "radius": "0.23",
      "beforeClearance": "0.175955",
      "afterClearance": "0.185371",
      "start": "<4511.441406, 340.270111, 10243.416016>",
      "end": "<4511.457031, 340.193146, 10243.427734>",
      "normal": "<0.659426, 0.385424, 0.645449>",
      "fraction": "0",
      "at": "1050",
      "nativeStartOverlapChecks": "0",
      "probeRole": "root0.9m",
      "stepDotHitNormal": -0.011797255526979356,
      "nativeInitialHit": true,
      "startBoxOverlapping": false,
      "endBoxDeeper": false,
      "fact": "Existing terrain-aware full-sweep gate rejects this precise probe;reject fields read without additional physics calls."
    },
    {
      "case": "3",
      "sourceSequence": "284",
      "id": "43",
      "mode": "2",
      "attempted": "true",
      "accepted": "false",
      "gate": "NEW_WORLD_CONTACT",
      "probe": "1",
      "fixture": "StaticObj_Container_1C",
      "fixtureIndex": "0",
      "fixturePosition": "<4510.000000, 338.823303, 10240.000000>",
      "oldLift": "0",
      "lift": "0",
      "radius": "0.23",
      "beforeClearance": "0.0956199",
      "afterClearance": "0.105448",
      "start": "<4511.488281, 340.277649, 10236.717773>",
      "end": "<4511.520020, 340.194214, 10236.717773>",
      "normal": "<0.867218, 0.496637, 0.035845>",
      "fraction": "0",
      "at": "1070",
      "nativeStartOverlapChecks": "0",
      "probeRole": "root0.9m",
      "stepDotHitNormal": -0.013912275992990431,
      "nativeInitialHit": true,
      "startBoxOverlapping": false,
      "endBoxDeeper": false,
      "fact": "Existing terrain-aware full-sweep gate rejects this precise probe;reject fields read without additional physics calls."
    }
  ],
  "planeResults": [
    {
      "case": "0",
      "sourceSequence": "226",
      "id": "4",
      "mode": "3",
      "attempted": "false",
      "gate": "NOT_REQUESTED",
      "firstAirGate": "NONDEEPENING_CLEAR",
      "accepted": "true",
      "firstNormal": "<0.890571, 0.332765, 0.310081>",
      "secondNormal": "<0.000000, 0.000000, 0.000000>",
      "firstProposal": "<4511.511719, 339.232849, 10243.349609>",
      "proposal": "<4511.511719, 339.232849, 10243.349609>",
      "after": "<4511.511719, 339.232849, 10243.349609>",
      "phase": "FALL",
      "finalAirGate": "NONDEEPENING_CLEAR",
      "movedMetres": 0.09412869067939313,
      "horizontalMetres": 0.031204242868733183,
      "stepDotFirstNormal": -0.00012957990085219829,
      "stepDotSecondNormal": 0.0
    },
    {
      "case": "1",
      "sourceSequence": "152",
      "id": "32",
      "mode": "3",
      "attempted": "false",
      "gate": "NOT_REQUESTED",
      "firstAirGate": "NONDEEPENING_CLEAR",
      "accepted": "true",
      "firstNormal": "<-0.004777, 0.255935, -0.966682>",
      "secondNormal": "<0.000000, 0.000000, 0.000000>",
      "firstProposal": "<4511.292969, 339.381714, 10236.494141>",
      "proposal": "<4511.292969, 339.381714, 10236.494141>",
      "after": "<4511.292969, 339.381714, 10236.494141>",
      "phase": "FALL",
      "finalAirGate": "NONDEEPENING_CLEAR",
      "movedMetres": 0.07550204636958478,
      "horizontalMetres": 0.019628402125403094,
      "stepDotFirstNormal": 0.00021173955092119306,
      "stepDotSecondNormal": 0.0
    },
    {
      "case": "2",
      "sourceSequence": "197",
      "id": "23",
      "mode": "3",
      "attempted": "true",
      "gate": "ACCEPTED_STILL_FALL",
      "firstAirGate": "NEW_WORLD_CONTACT",
      "accepted": "true",
      "firstNormal": "<0.673966, 0.247264, 0.696154>",
      "secondNormal": "<0.659426, 0.385424, 0.645449>",
      "firstProposal": "<4511.457031, 339.293152, 10243.427734>",
      "proposal": "<4511.453613, 339.367462, 10243.405273>",
      "after": "<4511.453613, 339.367462, 10243.405273>",
      "phase": "FALL",
      "finalAirGate": "NONDEEPENING_CLEAR",
      "movedMetres": 0.016476405037835505,
      "horizontalMetres": 0.016261085387230104,
      "stepDotFirstNormal": 9.183462059680295e-05,
      "stepDotSecondNormal": 9.225385554444306e-05
    },
    {
      "case": "3",
      "sourceSequence": "284",
      "id": "43",
      "mode": "3",
      "attempted": "true",
      "gate": "AIR_FULL_SWEEP",
      "firstAirGate": "NEW_WORLD_CONTACT",
      "accepted": "false",
      "firstNormal": "<0.933897, 0.357541, -0.000909>",
      "secondNormal": "<0.867218, 0.496637, 0.035845>",
      "firstProposal": "<4511.520020, 339.294220, 10236.717773>",
      "proposal": "<4511.490234, 339.373169, 10236.738281>",
      "after": "<4511.488281, 339.377655, 10236.717773>",
      "phase": "FALL",
      "finalAirGate": "NEW_WORLD_CONTACT",
      "movedMetres": 0.0,
      "horizontalMetres": 0.020600783309956596,
      "stepDotFirstNormal": 0.00020133014296592057,
      "stepDotSecondNormal": 0.00020087243195633935
    }
  ],
  "continuous": [
    {
      "case": 0,
      "sourceSequence": 226,
      "id": 4,
      "mode": 0,
      "steps": 240,
      "simulatedSeconds": 12.000000000000036,
      "physicalLand": false,
      "finalPhase": "FALL",
      "initialPosition": "<4511.482422, 339.321655, 10243.338867>",
      "finalPosition": "<4511.482422, 339.321655, 10243.338867>",
      "rootDisplacementMetres": 0.0,
      "candidateCalls": 0,
      "candidateMoves": 0,
      "stationarySuffixSeconds": 12.000000000000036,
      "lastReason": "FALL_NATIVE_JOINT_WORLD_0",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 240
      }
    },
    {
      "case": 0,
      "sourceSequence": 226,
      "id": 4,
      "mode": 1,
      "steps": 2,
      "simulatedSeconds": 0.1,
      "physicalLand": true,
      "finalPhase": "LAND",
      "initialPosition": "<4511.482422, 339.321655, 10243.338867>",
      "finalPosition": "<4511.517090, 339.181122, 10243.352539>",
      "rootDisplacementMetres": 0.14539022627740203,
      "candidateCalls": 1,
      "candidateMoves": 1,
      "stationarySuffixSeconds": 0.05,
      "lastReason": "NATIVE_FALL",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 1,
        "NATIVE_FALL": 1
      }
    },
    {
      "case": 1,
      "sourceSequence": 152,
      "id": 32,
      "mode": 0,
      "steps": 240,
      "simulatedSeconds": 6.716303999999984,
      "physicalLand": false,
      "finalPhase": "FALL",
      "initialPosition": "<4511.291016, 339.454620, 10236.513672>",
      "finalPosition": "<4511.291016, 339.454620, 10236.513672>",
      "rootDisplacementMetres": 0.0,
      "candidateCalls": 0,
      "candidateMoves": 0,
      "stationarySuffixSeconds": 6.716303999999984,
      "lastReason": "FALL_NATIVE_JOINT_WORLD_0",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 240
      }
    },
    {
      "case": 1,
      "sourceSequence": 152,
      "id": 32,
      "mode": 1,
      "steps": 2,
      "simulatedSeconds": 0.0559692,
      "physicalLand": true,
      "finalPhase": "LAND",
      "initialPosition": "<4511.291016, 339.454620, 10236.513672>",
      "finalPosition": "<4511.298828, 339.166870, 10236.494141>",
      "rootDisplacementMetres": 0.2885178500629918,
      "candidateCalls": 1,
      "candidateMoves": 1,
      "stationarySuffixSeconds": 0.0279846,
      "lastReason": "NATIVE_FALL",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 1,
        "NATIVE_FALL": 1
      }
    },
    {
      "case": 2,
      "sourceSequence": 197,
      "id": 23,
      "mode": 0,
      "steps": 240,
      "simulatedSeconds": 12.000000000000036,
      "physicalLand": false,
      "finalPhase": "FALL",
      "initialPosition": "<4511.441406, 339.370117, 10243.416016>",
      "finalPosition": "<4511.441406, 339.370117, 10243.416016>",
      "rootDisplacementMetres": 0.0,
      "candidateCalls": 0,
      "candidateMoves": 0,
      "stationarySuffixSeconds": 12.000000000000036,
      "lastReason": "FALL_NATIVE_JOINT_WORLD_0",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 240
      }
    },
    {
      "case": 2,
      "sourceSequence": 197,
      "id": 23,
      "mode": 1,
      "steps": 11,
      "simulatedSeconds": 0.5499999999999999,
      "physicalLand": true,
      "finalPhase": "LAND",
      "initialPosition": "<4511.441406, 339.370117, 10243.416016>",
      "finalPosition": "<4511.505859, 339.180969, 10243.329102>",
      "rootDisplacementMetres": 0.21791098758187383,
      "candidateCalls": 10,
      "candidateMoves": 10,
      "stationarySuffixSeconds": 0.05,
      "lastReason": "NATIVE_FALL",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 10,
        "NATIVE_FALL": 1
      }
    },
    {
      "case": 3,
      "sourceSequence": 284,
      "id": 43,
      "mode": 0,
      "steps": 240,
      "simulatedSeconds": 12.000000000000036,
      "physicalLand": false,
      "finalPhase": "FALL",
      "initialPosition": "<4511.488281, 339.377655, 10236.717773>",
      "finalPosition": "<4511.488281, 339.377655, 10236.717773>",
      "rootDisplacementMetres": 0.0,
      "candidateCalls": 0,
      "candidateMoves": 0,
      "stationarySuffixSeconds": 12.000000000000036,
      "lastReason": "FALL_NATIVE_JOINT_WORLD_0",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 240
      }
    },
    {
      "case": 3,
      "sourceSequence": 284,
      "id": 43,
      "mode": 1,
      "steps": 240,
      "simulatedSeconds": 12.000000000000036,
      "physicalLand": false,
      "finalPhase": "FALL",
      "initialPosition": "<4511.488281, 339.377655, 10236.717773>",
      "finalPosition": "<4511.488281, 339.377655, 10236.717773>",
      "rootDisplacementMetres": 0.0,
      "candidateCalls": 240,
      "candidateMoves": 0,
      "stationarySuffixSeconds": 12.000000000000036,
      "lastReason": "FALL_NATIVE_JOINT_WORLD_0",
      "lastLandingGate": "CLEAR",
      "reasonCounts": {
        "FALL_NATIVE_JOINT_WORLD_0": 240
      }
    }
  ],
  "repeatIdentical": true,
  "actualEpisodeResults": [
    {
      "sourceRunId": "runtime-20261029-191331",
      "id": 4,
      "generation": 1,
      "rejectedQuerySequence": 226,
      "querySeconds": 83.522,
      "actualLandTransition": {
        "sequence": "649",
        "seconds": "83.522",
        "id": "4",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "POST_TOUCHDOWN_POSE_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.489258, 339.180695, 10243.373047>",
        "contactStatus": "StaticObj_Container_1C",
        "contactBlocked": "false",
        "lift": "0",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0142242",
        "nativeSeconds": "37.3997",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "matchedLandingQuery": {
        "sequence": "227",
        "seconds": "83.522",
        "id": "4",
        "generation": "1",
        "phase": "FALL",
        "gate": "CLEAR",
        "valid": "true",
        "requireEntitySpace": "false",
        "root": "<4511.482422, 339.321655, 10243.369141>",
        "tested": "<4511.489258, 339.180695, 10243.373047>",
        "supported": "<4511.489258, 339.180695, 10243.373047>",
        "heading": "-114.719",
        "lift": "0",
        "reason": "FALL_CONTACT_AIR_SEPARATION",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "106",
        "rejected": "104",
        "budgetSkipped": "0",
        "poseAvailable": "true",
        "nativeBefore": "37.3997",
        "nativeAfter": "37.3997",
        "updatesBefore": "748",
        "updatesAfter": "748",
        "hipsMS": "<-0.004647, 0.978595, 0.000699>",
        "headMS": "<-0.006620, 1.631667, -0.222118>",
        "leftFootMS": "<0.161394, 0.187673, -0.142498>",
        "rightFootMS": "<-0.136910, 0.360476, -0.131073>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      },
      "actualFollowTransition": {
        "sequence": "726",
        "seconds": "96.18",
        "id": "4",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "ARRIVED",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.489258, 339.179596, 10244.190430>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0266418",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0142242",
        "nativeSeconds": "41.8996",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "timeToPhysicalLandSeconds": 0.0,
      "timeToFollowSeconds": 12.658000000000001,
      "rejectedAnchorToActualLandMetres": 0.03028974334538744,
      "classification": "Recovered actual episode;unmatched rejected anchor is not unresolved terminal failure."
    },
    {
      "sourceRunId": "runtime-20261029-193252",
      "id": 32,
      "generation": 1,
      "rejectedQuerySequence": 152,
      "querySeconds": 55.602,
      "actualLandTransition": {
        "sequence": "376",
        "seconds": "55.732",
        "id": "32",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "POST_TOUCHDOWN_POSE_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.299805, 339.166931, 10236.497070>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0",
        "nativeSeconds": "29.5998",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "matchedLandingQuery": {
        "sequence": "153",
        "seconds": "55.732",
        "id": "32",
        "generation": "1",
        "phase": "FALL",
        "gate": "CLEAR",
        "valid": "true",
        "requireEntitySpace": "false",
        "root": "<4511.291016, 339.454620, 10236.497070>",
        "tested": "<4511.299805, 339.166931, 10236.497070>",
        "supported": "<4511.299805, 339.166931, 10236.497070>",
        "heading": "-27.685",
        "lift": "0",
        "reason": "FRAME_FALL_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "64",
        "rejected": "62",
        "budgetSkipped": "0",
        "poseAvailable": "true",
        "nativeBefore": "29.5998",
        "nativeAfter": "29.5998",
        "updatesBefore": "592",
        "updatesAfter": "592",
        "hipsMS": "<-0.006577, 0.979465, -0.003685>",
        "headMS": "<-0.017316, 1.635520, -0.220732>",
        "leftFootMS": "<0.158207, 0.382690, -0.103425>",
        "rightFootMS": "<-0.193561, 0.223782, -0.340688>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      },
      "actualFollowTransition": {
        "sequence": "423",
        "seconds": "64.149",
        "id": "32",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "ARRIVED",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.299805, 339.164001, 10235.770508>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0325623",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0",
        "nativeSeconds": "33.9498",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "timeToPhysicalLandSeconds": 0.13000000000000256,
      "timeToFollowSeconds": 8.547000000000004,
      "rejectedAnchorToActualLandMetres": 0.016666541032499125,
      "classification": "Recovered actual episode;unmatched rejected anchor is not unresolved terminal failure."
    },
    {
      "sourceRunId": "runtime-20261029-193252",
      "id": 23,
      "generation": 1,
      "rejectedQuerySequence": 197,
      "querySeconds": 61.128,
      "actualLandTransition": {
        "sequence": "410",
        "seconds": "61.128",
        "id": "23",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "POST_TOUCHDOWN_POSE_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.449219, 339.179993, 10243.444336>",
        "contactStatus": "StaticObj_Container_1C",
        "contactBlocked": "false",
        "lift": "0",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0149855",
        "nativeSeconds": "32.2498",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "matchedLandingQuery": {
        "sequence": "198",
        "seconds": "61.128",
        "id": "23",
        "generation": "1",
        "phase": "FALL",
        "gate": "CLEAR",
        "valid": "true",
        "requireEntitySpace": "false",
        "root": "<4511.441406, 339.370117, 10243.446289>",
        "tested": "<4511.449219, 339.180023, 10243.444336>",
        "supported": "<4511.449219, 339.179993, 10243.444336>",
        "heading": "-140.013",
        "lift": "0",
        "reason": "FALL_CONTACT_AIR_SEPARATION",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "121",
        "rejected": "119",
        "budgetSkipped": "1",
        "poseAvailable": "true",
        "nativeBefore": "32.2498",
        "nativeAfter": "32.2498",
        "updatesBefore": "645",
        "updatesAfter": "645",
        "hipsMS": "<-0.008690, 0.978408, 0.001271>",
        "headMS": "<-0.030328, 1.633853, -0.215560>",
        "leftFootMS": "<0.163394, 0.205418, -0.243121>",
        "rightFootMS": "<-0.125196, 0.309381, -0.012137>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      },
      "actualFollowTransition": {
        "sequence": "463",
        "seconds": "69.895",
        "id": "23",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "ARRIVED",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.449219, 339.178986, 10244.201172>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0235596",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0149855",
        "nativeSeconds": "36.6497",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "timeToPhysicalLandSeconds": 0.0,
      "timeToFollowSeconds": 8.766999999999996,
      "rejectedAnchorToActualLandMetres": 0.030273014865033917,
      "classification": "Recovered actual episode;unmatched rejected anchor is not unresolved terminal failure."
    },
    {
      "sourceRunId": "runtime-20261029-193252",
      "id": 43,
      "generation": 1,
      "rejectedQuerySequence": 284,
      "querySeconds": 81.208,
      "actualLandTransition": {
        "sequence": "539",
        "seconds": "81.208",
        "id": "43",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "POST_TOUCHDOWN_POSE_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.516113, 339.171753, 10236.696289>",
        "contactStatus": "StaticObj_Container_1C",
        "contactBlocked": "false",
        "lift": "0",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0149855",
        "nativeSeconds": "41.8496",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "matchedLandingQuery": {
        "sequence": "285",
        "seconds": "81.208",
        "id": "43",
        "generation": "1",
        "phase": "FALL",
        "gate": "CLEAR",
        "valid": "true",
        "requireEntitySpace": "false",
        "root": "<4511.509277, 339.377655, 10236.696289>",
        "tested": "<4511.516113, 339.171753, 10236.696289>",
        "supported": "<4511.516113, 339.171753, 10236.696289>",
        "heading": "-105.053",
        "lift": "0",
        "reason": "FALL_CONTACT_AIR_SEPARATION",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "118",
        "rejected": "116",
        "budgetSkipped": "0",
        "poseAvailable": "true",
        "nativeBefore": "41.8496",
        "nativeAfter": "41.8496",
        "updatesBefore": "837",
        "updatesAfter": "837",
        "hipsMS": "<-0.000604, 0.978783, 0.000126>",
        "headMS": "<0.016024, 1.626527, -0.230804>",
        "leftFootMS": "<0.165179, 0.216643, -0.021893>",
        "rightFootMS": "<-0.155917, 0.373479, -0.284418>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      },
      "actualFollowTransition": {
        "sequence": "626",
        "seconds": "91.58",
        "id": "43",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "ARRIVED",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.516113, 339.167999, 10235.757813>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.031189",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0149855",
        "nativeSeconds": "46.5496",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      "timeToPhysicalLandSeconds": 0.0,
      "timeToFollowSeconds": 10.372,
      "rejectedAnchorToActualLandMetres": 0.029366617817129445,
      "classification": "Recovered actual episode;unmatched rejected anchor is not unresolved terminal failure."
    }
  ],
  "classificationCorrection": "Fixed-pose fallback-suppressed candidate tests isolate contact geometry;they do not show actual original recovery failing. Do not promote candidate from3/4syntheticLAND overactual4/4recovered episodes.",
  "selection": "All4previouscases;notwhole64logs."
}

Selected generated helper class;notdeployed:
class DGCapturedContinuousTraversal: DGCapturedContactTraversal
{
 int AcceptCalls;bool LastAccepted;vector LastAttempt;string CandidateGate;string CandidateAirGate;
 int CandidateCalls;int CandidateMoves;
 void DGCapturedContinuousTraversal(DGBNativeGroundBody body){Body=body;PhaseAt=body.Director.Now;}
 override bool Accept(vector next,float dt)
 {
  AcceptCalls++;LastAttempt=next;LastAccepted=super.Accept(next,dt);return LastAccepted;
 }
 bool TryPlanes(vector target,float dt)
 {
  DGBNativeGroundBody body=Body;DGCapturedContinuousTraversal traversal=this;
  bool candidateAttempt=false;bool candidateAccepted=false;string candidateGate="NOT_REQUESTED";
  vector proposed=body.Agent.Position;vector beforeCandidate=body.Agent.Position;
  if( traversal.Phase=="FALL" && body.Reason=="FALL_NATIVE_JOINT_WORLD_0")
  {
   candidateAttempt=true;candidateGate="NORMAL";
   vector contactNormal=body.HitNormal;float contactFraction=body.HitFraction;
   if(contactNormal.Length()>0.99 && contactNormal.Length()<1.01 && contactFraction>=0 && contactFraction<=1)
   {
    vector remaining=(target - body.Agent.Position)*(1 - contactFraction);
    vector slide=remaining - contactNormal*vector.Dot(remaining,contactNormal);
    candidateGate="DOWNWARD";
    vector recordedSupport=target;vector currentRoot=body.Agent.Position;
    if(slide[1]<=0 && slide[1]>=recordedSupport[1] - currentRoot[1])
    {
     float horizontal=Math.Sqrt(slide[0]*slide[0]+slide[2]*slide[2]);
     float recordedDt=dt;float limit=Math.Min(0.06,1.2*recordedDt);
     if(horizontal>limit){float scale=limit/horizontal;slide=slide*scale;}
     proposed=body.Agent.Position+slide;
     float proposedLift;ref array<vector> proposedContacts={};candidateGate="AIR_FULL_SWEEP";
     if(traversal.PressureEscapeStepClear(proposed,proposedLift,proposedContacts))
     {
      candidateGate="ACCEPT_FULL_CONTACT";
      candidateAccepted=traversal.Accept(proposed,dt);
      if(candidateAccepted)candidateGate="ACCEPTED_STILL_FALL";
     }
    }
   }
  }
  bool secondAttempt=false;string secondGate="NOT_REQUESTED";
  vector firstNormal=body.HitNormal;vector secondNormal=traversal.PressureEscapeRejectNormal;
  string firstAirGate=traversal.PressureEscapeDiagnostic;vector firstProposal=proposed;
  if( !candidateAccepted && firstAirGate=="NEW_WORLD_CONTACT" && traversal.PressureEscapeRejectAt==Body.Director.Now)
  {
   secondAttempt=true;secondGate="NORMALS";
   float firstLength=firstNormal.Length();float secondLength=secondNormal.Length();
   if(firstLength>0.99 && firstLength<1.01 && secondLength>0.99 && secondLength<1.01)
   {
    vector crease=Vector(firstNormal[1]*secondNormal[2] - firstNormal[2]*secondNormal[1],firstNormal[2]*secondNormal[0] - firstNormal[0]*secondNormal[2],firstNormal[0]*secondNormal[1] - firstNormal[1]*secondNormal[0]);
    float lengthSquared=vector.Dot(crease,crease);secondGate="INDEPENDENT_PLANES";
    if(lengthSquared>0.0001)
    {
     vector sourceSupport=target;vector currentPosition=body.Agent.Position;
     vector remainder=(sourceSupport - currentPosition)*(1 - body.HitFraction);
     float along=vector.Dot(remainder,crease)/lengthSquared;vector creaseStep=crease*along;
     secondGate="DOWNWARD";
     if(creaseStep[1]<=0 && creaseStep[1]>=sourceSupport[1] - currentPosition[1])
     {
      float creaseHorizontal=Math.Sqrt(creaseStep[0]*creaseStep[0]+creaseStep[2]*creaseStep[2]);
      float sourceDt=dt;float creaseLimit=Math.Min(0.06,1.2*sourceDt);
      if(creaseHorizontal>creaseLimit){float creaseScale=creaseLimit/creaseHorizontal;creaseStep=creaseStep*creaseScale;}
      secondGate="BOTH_NORMALS";
      if(vector.Dot(creaseStep,firstNormal)>=-0.000001 && vector.Dot(creaseStep,secondNormal)>=-0.000001)
      {
       proposed=currentPosition+creaseStep;float creaseLift;ref array<vector> creaseContacts={};secondGate="AIR_FULL_SWEEP";
       if(traversal.PressureEscapeStepClear(proposed,creaseLift,creaseContacts))
       {
        secondGate="ACCEPT_FULL_CONTACT";
        candidateAccepted=traversal.Accept(proposed,sourceDt);
        if(candidateAccepted){secondGate="ACCEPTED_STILL_FALL";candidateGate="ACCEPTED_TWO_PLANE_STILL_FALL";}
       }
      }
     }
    }
   }
  }

  CandidateCalls++;CandidateGate=candidateGate+"/"+secondGate;CandidateAirGate=PressureEscapeDiagnostic;
  if(candidateAccepted)CandidateMoves++;return candidateAccepted;
 }
}


이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
