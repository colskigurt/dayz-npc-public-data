# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T043805-owner-cost-clock-20261029

64명 콜백 시간0.497배 — 미도달 유형과 비용 범위 분리

작성: 2026-10-29T13:35:37.752964+09:00

1. 작업 목표

종료된 BB45A140 시험 기록으로 64명 고지대 미도달24명의 활동과 느린 콜백 시간을 구분한다. 기존 누적 타이머의 범위를 실제 소스와 대조해 비용을 귀속한다. 분석 결과만으로 새 보간·배속·이동 정책이나 추측 최적화를 적용하지 않는다.

2. 실제 수행

runtime-20261029-130025(8명),130645(64명)의 기존300초 기록을 재분석했다. 새 게임 실행·녹화·PBO 수정·메모리 쓰기는 없다. 모두 활성 창, 같은BB45 빌드·씬·그래픽 seed/VSync·상세 계측 조건이다. 개체 수가 달라 경로·접촉·표시 작업량도 달랐으며 각각1회 관측이다.

DriveAttribution이 OFF인 이 기록에도 cpuClockCounter와 기존 타이머가 있다. actor0의 foreground 연속쌍만 골라 unsigned32 시계 차이를 GetTickTime 구간 합으로 나눴다. 전 개체 행에 반복된 공통 타이머는 한 번만, 개체별 타이머는 각 개체의 증가량만 합산했다. 시계 wrap·중복 actor·역순/배경 구간 거부 검사3개가 통과했다. 기존 drive 시계를 쓰는 분석 경로는 유지했다.

별도 도구로 supplied mission dt와 실제 미션 타임스탬프 간격, NativeSeconds·AnimUpdates 증가를 대조했다. 고지대 미도달은 높은 면으로의 MOTION 시작·샘플, 지지 상실 낙하, 표본 위치 변화로 구분했다. 원시 기록과 기존 보고서는 변경하지 않았다.

3. 측정 결과

|항목|8명|64명|
|---|---:|---:|
|네이티브 콜백 시간/미션 시간|0.770983|0.496957|
|콜백 호출/초|15.419040|9.938794|

8명14738쌍은 미션294.960초 동안 콜백 시간227.409초,64명3131쌍은 미션294.905초 동안146.555초 진행했다. 콜백당 증가량은 양쪽 모두 약0.050002초였다. CPU 콜백의 시간 누적이며 직접 clip normalized phase나 실제 화면 재생률은 아니다.

같은 foreground쌍의 타임스탬프 간격 p50/p95/p99는8명20/32/34ms,64명79/193/271ms였다.64명의 supplied dt는 최대300ms였지만 타임스탬프 간격은 최대393ms였다.300ms 근처 dt15개와301ms 초과 간격15개를 보존했다. 이 관측만으로 엔진 내부 clamp 위치를 특정하지 않는다. 이전 supplied dt 기반 p95194ms를 덮어쓰거나 GPU Present 값으로 바꾸지 않는다.

|HIGH_HOLD 측정 범위|8명 ms/s|64명 ms/s|
|---|---:|---:|
|인식 하위 범위|2.048|50.087|
|legacy 지면 이동 범위|23.003|231.479|
|접촉 전체 범위|35.649|225.671|
|접촉 안의 본 조회|1.254|5.901|
|접촉 안의 발 지지|21.394|129.521|
|접촉 안의 월드 검사|11.171|79.924|
|legacy trace 기록 범위|12.862|62.241|
|Crowd.Update 범위|6.431|12.283|

위 값은 wall second당 elapsed-equivalent ms이며, 중첩 범위를 합산한 CPU self-time이 아니다. HIGH_HOLD 유효 구간은8명59.405초·64명59.675초다. 시계 보정값은 각각 약9,999,983·10,001,202 ticks/s였고 각1회 wrap을 처리했다. 단일 짧은 구간의 보정률 변동도 결과 JSON에 유지했다.

고지대 미도달24명 중23명은45.012–104.989초 구간에서 low_to_high MOTION 시작/표본이 관측되지 않았고,44번1명은 구간 종료 전에 등반을 시작했다.24명 모두 표본 경로 이동량0.5m 이상이었다.5명에는 지지 상실에 의한 FALL 전환이 있었다. 이동량은 정상 이동·밀림을 함께 포함하고, 모든 시간 동안 계속 움직였다는 증거가 아니다.

44번:99.237초 MOTION 시작→107.686초 높은 면 SETTLE→109.311초 FOLLOW. 논리 clip2.13333초, MotionElapsed2.15초를 실행하는 데 실제8.449초가 걸렸다. 실패 라벨은 없고 구간 종료 뒤 안전하게 완료했다.17번:51.462초 지지 상실 FALL→91.756초 LAND. 긴 LANDING_SPACE_BLOCKED 표본과 최장31.535초의 동일 위치 표본 구간이 있었다. 방향은 변했으므로 단순 root 정지를 모든 AI 정지로 정의하지 않는다.

이전 기능 결과는 유지한다:8/8·64/64 최종 도착, 고지대 미도달0/24, 복귀한 낙하10/56, 등반 종료 실패0/0. 새 기능 회귀나 최적화 반복을 수행한 것은 아니다.20ms p95는 미달이다.

4. 원인 분석

확인:64명 기록의 콜백 누적 시간은 미션 시간의 약0.497배다. 이전1개체 고정run의 약0.65와 다른 조건이며, 사용자가 느낀0.5배를 이 기록에서 뒷받침하는 시간 전달 증거다. 실제 화면의 같은 장면을 재측정했다고 주장하지 않는다.44번은 논리 동작 시간이 진행되는 동안 훨씬 긴 wall time이 걸려 고지대 관측 창을 넘겼다.

확인: 선택한 접촉 범위에서 본 조회보다 발 지지·월드 검사가 큰 elapsed 값을 보였다. 인식과 trace 기록도 측정 가능한 비용이 있었다. driveTicks는 지면 이동 하위 범위이며 Body.Drive 전체가 아니다. contact는 Body.Drive 바깥에서 호출될 수도 있다. 남은 시간을 렌더 비용으로 부르거나 여러 타이머를 더해 전체 원인을 설명하지 않는다.

추정: 낮은 callback 빈도와 시간 공급이 접근·등반 지연에 기여할 수 있다. 그러나23명의 등반 미시작,17번의 장시간 착지 차단, 군집 압력은 각각 구체적인 게이트 증거가 더 필요하다. 본 조회만 줄이면 전체 문제가 해결된다는 근거는 이번 선택 범위에서 나오지 않았다.

계측 한계: 현재 계약은 phase 전환을 주로 기록한다. FOLLOW 상태가 유지되는 Backoff 호출의 정확한 횟수/사유와 FALL 중 LandingSpace 실패의 구체적인 거부 게이트·해당 개체 포즈는 모두 남아 있지 않다. reason 표본 수를 정확한 backoff 횟수나 실패 지속 시간으로 바꾸지 않는다.

5. 검증 수준

진단 성공:저장된 기록의 시계·중첩 비용·활동 그룹을 분리했고 원시 해시와 소스 범위를 연결했다. 도구 회귀 통과:unsigned 시계·중복 공통 카운터·유효 구간 검사3개. 반복 검증 완료:새 실행/효과 반복이 없어 해당하지 않음. 최종 수용:기능·시각·성능 모두 미수용이다.

0.497은 콜백 시간 비율이다. 실제 visible frame·GPU·direct graph owner 시계는 이번에 추가 계측하지 않았다. 특정 기능의 비용 기여도를 인과적으로 분리한 OFF/ON 시험도 미실행이다.

6. 변경 사항

analyze_horde_legacy_cost_scopes.py에 ground 시계 선택 경로와 기존 출력 덮어쓰기 방지를 추가했다. callback/프레임 시계 감사, 고지대 미도달 활동 그룹 분석, 시계 보정 검사 도구를 추가했다. 이전 도구 원문과 이번 producer를 별도로 보존했다.

게임 소스·배포 PBO·애니메이션 자산·엔진 설정은 변경하지 않았다. 기존 BB45 빌드와 그 서명 검증 결과를 사용했으며 새 빌드·새 서명 작업은 수행하지 않았다. 운영 서버는 변경하지 않았다.

7. 원시 증거

두 실행의 launch·native-ground·native-pose-cadence·자극·상태 계약·기존 분석에 새 legacy-cost-scopes.json/frame-clock-audit.json/high-miss-groups.json을 연결한다.234개 근거를 보존한 이전 회귀 보고서의 원시 해시는 유지한다. 새 보고서는 직접 사용하는 파일을 저장 전후 해시·JSON·metadata 안정성·안전 종료로 검증한다.

selected-examples.json은44번98–116초와17번50–100초의 모든 TRANSITION 행을 필드 투영한 것이다. 전체 개체 이력은 아니다. 공개 검토 패킷은 선정 수치·자체 분석 함수·대표 전환만 포함하며 전체 trace·영상·설치 자산·내부 경로·프로세스 정보는 공개하지 않는다.

8. 잔여 문제

기능:17번 같은 착지 차단 개체에서 LandingSpace의 거부 게이트와 포즈, 동일 phase의 Backoff 호출을 제한된 주기로 계측해야 한다. 관측 누락을 메우기 전 접촉 검사를 완화하거나 timeout을 늘리지 않는다.44번의 늦은 완료를 충돌 실패와 분리한다.

성능:기존 반복 비용 매트릭스와 소스를 먼저 대조한 뒤 같은 빌드·조건에서 한 요소만 바꾸는 계측/비용 시험을 설계한다. 본 조회·trace·접촉을 한꺼번에 바꾸지 않는다. AI 자체 호출 빈도, 전체 Body.Drive, 전체 미션 갱신, native graph self-time, skinning/upload/GPU, 할당·GC는 이 기록에서 직접 분리할 수 없다. 특히 runtimeTimingProbe=false여서 미션 전체 timer는0이 아니라 미계측이다.

애니메이션:기존 고정50ms 전달/초과분 회계와 이번 callback 비율을 연결하되 지원되는 수정 수단과 실제 표시 품질을 별도로 확인해야 한다. 임의 배속이나 새 보간을 채택하지 않았다. 새 확장 복구의 실제 군집 분기 실행과64명 반복 회귀도 아직 필요하다.

원본 보고서 ID: 2026-10-29_133537_752964+0900_npc-owner-cost-clock.md
원본 SHA-256: 1fb54ffb192f22d24390d357189eb44d4aa649217fe68a239ed2bd7601b2ab05
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selected derived evidence; not full trace or visible-quality proof
{
  "clock": [
    {
      "runId": "runtime-20261029-130025",
      "buildId": "BB45A140982F623BEE628C9B26353A997EF6DAB9DF5F43D71E60BC4910DA805F",
      "count": 8,
      "foregroundPairs": 14738,
      "excluded": {
        "warmup": 225
      },
      "missionTimestampGapsMs": {
        "n": 14738,
        "p50": 20.000000000003126,
        "p95": 31.999999999982265,
        "p99": 33.999999999991815,
        "maximum": 46.999999999997044,
        "minimum": 6.999999999990791
      },
      "suppliedMissionDtMs": {
        "n": 14738,
        "p50": 20.0,
        "p95": 31.0,
        "p99": 33.0,
        "maximum": 47.0,
        "minimum": 6.0
      },
      "timestampMinusSuppliedMs": {
        "n": 14738,
        "p50": 4.5449755070592346e-13,
        "p95": 1.0000000000150062,
        "p99": 2.0000000000059104,
        "maximum": 10.000000000006365,
        "minimum": -6.0000000000036415
      },
      "reportedDtNear300Count": 0,
      "timestampGapsOver301Count": 0,
      "selectedLongGapPairs": [],
      "callbackClock": {
        "observedMissionSeconds": 294.96000000001,
        "nativeAdvanceSeconds": 227.409,
        "calls": 4548,
        "nativeClockPerMissionSecond": 0.7709825061024963,
        "callbackCallsPerMissionSecond": 15.419039869812334,
        "nativeAdvancePerCallback": 0.05000197889182058
      },
      "limitations": [
        "GetTickTime timestamp differences and supplied mission timeslice are distinct; neither is GPU Present.",
        "Callback NativeSeconds accumulates PreAnim pDt; not independently normalized clip phase or visible smoothness.",
        "Intervals are foreground observations of actor0/part0 with instrumentation. No causal optimization effect.",
        "Long gaps alone do not identify engine clamping location, scheduler wait or render/AI ownership."
      ],
      "visualAccepted": false,
      "performanceAccepted": false
    },
    {
      "runId": "runtime-20261029-130645",
      "buildId": "BB45A140982F623BEE628C9B26353A997EF6DAB9DF5F43D71E60BC4910DA805F",
      "count": 64,
      "foregroundPairs": 3131,
      "excluded": {
        "warmup": 74
      },
      "missionTimestampGapsMs": {
        "n": 3131,
        "p50": 78.99999999997931,
        "p95": 193.00000000001205,
        "p99": 270.9999999999866,
        "maximum": 393.0000000000007,
        "minimum": 16.000000000019554
      },
      "suppliedMissionDtMs": {
        "n": 3131,
        "p50": 79.0,
        "p95": 194.0,
        "p99": 271.0,
        "maximum": 300.0,
        "minimum": 16.0
      },
      "timestampMinusSuppliedMs": {
        "n": 3131,
        "p50": -1.3877787807814457e-13,
        "p95": 4.000000000022736,
        "p99": 39.00000000000579,
        "maximum": 93.0000000000007,
        "minimum": -60.999999999992504
      },
      "reportedDtNear300Count": 15,
      "timestampGapsOver301Count": 15,
      "selectedLongGapPairs": [
        {
          "seconds": 142.403,
          "timestampGapMs": 393.0000000000007,
          "suppliedDtMs": 300.0,
          "residualMs": 93.0000000000007,
          "nativeAdvanceSeconds": 0.05000000000001137,
          "callbackCalls": 1
        },
        {
          "seconds": 162.417,
          "timestampGapMs": 381.0000000000002,
          "suppliedDtMs": 300.0,
          "residualMs": 81.00000000000024,
          "nativeAdvanceSeconds": 0.05000000000001137,
          "callbackCalls": 1
        },
        {
          "seconds": 161.305,
          "timestampGapMs": 378.0000000000143,
          "suppliedDtMs": 300.0,
          "residualMs": 78.00000000001434,
          "nativeAdvanceSeconds": 0.05000000000001137,
          "callbackCalls": 1
        },
        {
          "seconds": 101.142,
          "timestampGapMs": 350.9999999999991,
          "suppliedDtMs": 300.0,
          "residualMs": 50.999999999999105,
          "nativeAdvanceSeconds": 0.05000000000000426,
          "callbackCalls": 1
        },
        {
          "seconds": 140.672,
          "timestampGapMs": 349.9999999999943,
          "suppliedDtMs": 300.0,
          "residualMs": 49.99999999999433,
          "nativeAdvanceSeconds": 0.05000000000001137,
          "callbackCalls": 1
        },
        {
          "seconds": 164.514,
          "timestampGapMs": 332.00000000002206,
          "suppliedDtMs": 300.0,
          "residualMs": 32.00000000002207,
          "nativeAdvanceSeconds": 0.04999999999999716,
          "callbackCalls": 1
        },
        {
          "seconds": 162.889,
          "timestampGapMs": 328.00000000000296,
          "suppliedDtMs": 300.0,
          "residualMs": 28.000000000002967,
          "nativeAdvanceSeconds": 0.04999999999999716,
          "callbackCalls": 1
        },
        {
          "seconds": 141.791,
          "timestampGapMs": 323.99999999998386,
          "suppliedDtMs": 300.0,
          "residualMs": 23.999999999983867,
          "nativeAdvanceSeconds": 0.04999999999999716,
          "callbackCalls": 1
        },
        {
          "seconds": 169.285,
          "timestampGapMs": 323.0000000000075,
          "suppliedDtMs": 300.0,
          "residualMs": 23.000000000007514,
          "nativeAdvanceSeconds": 0.04999999999999716,
          "callbackCalls": 1
        },
        {
          "seconds": 137.957,
          "timestampGapMs": 319.9999999999932,
          "suppliedDtMs": 300.0,
          "residualMs": 19.99999999999319,
          "nativeAdvanceSeconds": 0.04999999999999716,
          "callbackCalls": 1
        }
      ],
      "callbackClock": {
        "observedMissionSeconds": 294.9050000000001,
        "nativeAdvanceSeconds": 146.55499999999998,
        "calls": 2931,
        "nativeClockPerMissionSecond": 0.4969566470558313,
        "callbackCallsPerMissionSecond": 9.938793848866581,
        "nativeAdvancePerCallback": 0.05000170590242237
      },
      "limitations": [
        "GetTickTime timestamp differences and supplied mission timeslice are distinct; neither is GPU Present.",
        "Callback NativeSeconds accumulates PreAnim pDt; not independently normalized clip phase or visible smoothness.",
        "Intervals are foreground observations of actor0/part0 with instrumentation. No causal optimization effect.",
        "Long gaps alone do not identify engine clamping location, scheduler wait or render/AI ownership."
      ],
      "visualAccepted": false,
      "performanceAccepted": false
    }
  ],
  "highGroups": {
    "runId": "runtime-20261029-130645",
    "buildId": "BB45A140982F623BEE628C9B26353A997EF6DAB9DF5F43D71E60BC4910DA805F",
    "highWindow": [
      45.012,
      104.989
    ],
    "missing": 24,
    "groups": {
      "no_high_climb_motion_observed": 23,
      "high_climb_motion_without_high_follow": 1
    },
    "actors": [
      {
        "id": 2,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 1,
        "pressureSupportLossTransitions": 1,
        "sampledPathTravelMeters": 25.56429912351455,
        "distinctRootSamples": 98,
        "nonzeroMeasuredSpeedSamples": 53,
        "longestUnchangedSampleSpanSeconds": 3.7509999999999977,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 19,
          "LINK_ALIGN": 4
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 83,
          "FALL": 10,
          "LAND": 12,
          "EGRESS": 4
        },
        "reasonCounts": {
          "": 51,
          "ALIGN_HEADING": 19,
          "LANDING_SPACE_BLOCKED": 6,
          "FALL_CONTACT_AIR_SEPARATION": 4,
          "LAND_CLEARANCE_SEPARATION": 1,
          "LAND": 11,
          "RECOVERY_REPLAN_WAIT": 4,
          "EGRESS_POSE_CLEARANCE": 2,
          "EGRESS_POSE_CLEARANCE_DONE": 1,
          "LINK_ALIGN": 4,
          "ENTRY_POSE_SETTLE": 3,
          "NO_GROUND_PATH": 1,
          "SUPPORT_BYPASS_PLANNED": 2
        }
      },
      {
        "id": 4,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 1,
        "pressureSupportLossTransitions": 1,
        "sampledPathTravelMeters": 19.486034481755844,
        "distinctRootSamples": 101,
        "nonzeroMeasuredSpeedSamples": 44,
        "longestUnchangedSampleSpanSeconds": 3.9299999999999926,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 15,
          "LINK_ALIGN": 16,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 82,
          "MOTION": 6,
          "SETTLE": 2,
          "FALL": 6,
          "LAND": 11,
          "EGRESS": 2
        },
        "reasonCounts": {
          "ALIGN_HEADING": 15,
          "": 42,
          "LINK_ALIGN": 16,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2,
          "RECOVERY_REPLAN_WAIT": 3,
          "NATIVE_MOTION": 6,
          "SETTLE": 2,
          "LANDING_SPACE_BLOCKED": 5,
          "FALL_CONTACT_AIR_SEPARATION": 1,
          "LAND_CLEARANCE_SEPARATION": 2,
          "LAND": 10,
          "NATIVE_JOINT_WORLD_1": 1,
          "POSE_CLEARANCE_SEPARATION": 4
        }
      },
      {
        "id": 6,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 1,
        "pressureSupportLossTransitions": 1,
        "sampledPathTravelMeters": 24.850828544802315,
        "distinctRootSamples": 100,
        "nonzeroMeasuredSpeedSamples": 52,
        "longestUnchangedSampleSpanSeconds": 3.811,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 12,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 91,
          "FALL": 6,
          "LAND": 11,
          "EGRESS": 1
        },
        "reasonCounts": {
          "": 50,
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 12,
          "FOOTPRINT_UNSUPPORTED": 1,
          "RECOVERY_REPLAN_WAIT": 2,
          "NATIVE_FALL": 1,
          "LANDING_SPACE_BLOCKED": 5,
          "LAND": 8,
          "LAND_CLEARANCE_SEPARATION": 3,
          "NATIVE_JOINT_WORLD_1": 1,
          "POSE_CLEARANCE_SEPARATION": 13,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "NO_GROUND_PATH": 1
        }
      },
      {
        "id": 8,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 18.89232952983047,
        "distinctRootSamples": 108,
        "nonzeroMeasuredSpeedSamples": 47,
        "longestUnchangedSampleSpanSeconds": 0.5290000000000035,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 18,
          "LINK_ALIGN": 17,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 4
        },
        "finalArrived": true,
        "phaseCounts": {
          "EGRESS": 6,
          "FOLLOW": 103
        },
        "reasonCounts": {
          "RECOVERY_REPLAN_WAIT": 10,
          "EGRESS_POSE_CLEARANCE": 5,
          "ALIGN_HEADING": 18,
          "": 51,
          "LINK_ALIGN": 17,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 4,
          "ENTRY_POSE_SETTLE": 2,
          "SUPPORT_BYPASS_PLANNED": 1,
          "NO_GROUND_PATH": 1
        }
      },
      {
        "id": 11,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 18.529710898576553,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 48,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 29,
          "LINK_ALIGN": 10,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "ALIGN_HEADING": 29,
          "": 60,
          "LINK_ALIGN": 10,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "RECOVERY_REPLAN_WAIT": 9
        }
      },
      {
        "id": 12,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 16.629561822078056,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 45,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 24,
          "LINK_ALIGN": 8,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "SUPPORT_BYPASS_PLANNED": 5,
          "ALIGN_HEADING": 24,
          "": 58,
          "LINK_ALIGN": 8,
          "ENTRY_POSE_SETTLE": 4,
          "RECOVERY_REPLAN_WAIT": 7,
          "NO_GROUND_PATH": 2,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        }
      },
      {
        "id": 17,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 1,
        "pressureSupportLossTransitions": 1,
        "sampledPathTravelMeters": 6.801783572641715,
        "distinctRootSamples": 32,
        "nonzeroMeasuredSpeedSamples": 17,
        "longestUnchangedSampleSpanSeconds": 31.534999999999997,
        "alignmentReasonSamples": {
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "ALIGN_HEADING": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 24,
          "FALL": 74,
          "LAND": 11
        },
        "reasonCounts": {
          "ARRIVED": 2,
          "": 7,
          "ENTRY_POSE_SETTLE": 1,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "ALIGN_HEADING": 2,
          "FALL_CONTACT_AIR_SEPARATION": 1,
          "NATIVE_FALL": 1,
          "LANDING_SPACE_BLOCKED": 72,
          "LAND_CLEARANCE_SEPARATION": 1,
          "LAND": 10,
          "NATIVE_JOINT_WORLD_0": 6,
          "RECOVERY_REPLAN_WAIT": 1,
          "POSE_CLEARANCE_SEPARATION": 3,
          "SUPPORTED_POSE_ESCAPE_TURN": 1
        }
      },
      {
        "id": 23,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 16.64524201117152,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 38,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 8,
          "ALIGN_HEADING": 25,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "LINK_ALIGN": 8,
          "ALIGN_HEADING": 25,
          "": 53,
          "RECOVERY_REPLAN_WAIT": 12,
          "SUPPORT_BYPASS_PLANNED": 4,
          "NO_GROUND_PATH": 4,
          "ENTRY_POSE_SETTLE": 2,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        }
      },
      {
        "id": 27,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.76724643335262,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 47,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 20,
          "ALIGN_HEADING": 29
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "LINK_ALIGN": 20,
          "ALIGN_HEADING": 29,
          "": 54,
          "RECOVERY_REPLAN_WAIT": 3,
          "EXIT_LEASE_BACKOFF": 1,
          "SUPPORT_BYPASS_PLANNED": 1,
          "NO_GROUND_PATH": 1
        }
      },
      {
        "id": 31,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 20.94406286053522,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 44,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 35,
          "LINK_ALIGN": 7,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "": 53,
          "ALIGN_HEADING": 35,
          "RECOVERY_REPLAN_WAIT": 6,
          "LINK_ALIGN": 7,
          "SUPPORT_BYPASS_PLANNED": 2,
          "NO_GROUND_PATH": 3,
          "ENTRY_POSE_SETTLE": 1,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        }
      },
      {
        "id": 34,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 1,
        "pressureSupportLossTransitions": 1,
        "sampledPathTravelMeters": 16.822639576019114,
        "distinctRootSamples": 95,
        "nonzeroMeasuredSpeedSamples": 38,
        "longestUnchangedSampleSpanSeconds": 4.352999999999994,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 8,
          "ALIGN_HEADING": 23,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 81,
          "FALL": 15,
          "LAND": 13
        },
        "reasonCounts": {
          "ARRIVED": 2,
          "": 33,
          "LINK_ALIGN": 8,
          "ALIGN_HEADING": 23,
          "NATIVE_JOINT_WORLD_3": 1,
          "POSE_CLEARANCE_SEPARATION": 3,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "LANDING_SPACE_BLOCKED": 9,
          "NATIVE_FALL": 2,
          "FALL_CONTACT_AIR_SEPARATION": 2,
          "FALL_NATIVE_JOINT_WORLD_0": 3,
          "LAND": 11,
          "LAND_CLEARANCE_SEPARATION": 1,
          "RECOVERY_REPLAN_WAIT": 5,
          "NATIVE_JOINT_WORLD_0": 1,
          "NO_GROUND_PATH": 3,
          "SUPPORT_BYPASS_PLANNED": 1
        }
      },
      {
        "id": 39,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 16.92403776701535,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 33,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 26,
          "ALIGN_HEADING": 28,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "LINK_ALIGN": 26,
          "": 43,
          "ALIGN_HEADING": 28,
          "RECOVERY_REPLAN_WAIT": 7,
          "ENTRY_POSE_SETTLE": 1,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2,
          "NO_GROUND_PATH": 1,
          "SUPPORT_BYPASS_PLANNED": 1
        }
      },
      {
        "id": 43,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.56855505993375,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 44,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 20,
          "LINK_ALIGN": 17
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "ALIGN_HEADING": 20,
          "": 58,
          "LINK_ALIGN": 17,
          "ENTRY_POSE_SETTLE": 5,
          "NO_GROUND_PATH": 2,
          "SUPPORT_BYPASS_PLANNED": 2,
          "RECOVERY_REPLAN_WAIT": 5
        }
      },
      {
        "id": 44,
        "observedGroup": "high_climb_motion_without_high_follow",
        "samples": 109,
        "highClimbStarts": 1,
        "highClimbMotionSamples": 9,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 19.06173643988445,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 59,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 13,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 93,
          "MOTION": 14,
          "SETTLE": 2
        },
        "reasonCounts": {
          "": 54,
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 13,
          "SUPPORT_BYPASS_PLANNED": 3,
          "RECOVERY_REPLAN_WAIT": 9,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "ENTRY_POSE_SETTLE": 2,
          "NATIVE_MOTION": 15,
          "SETTLE": 1
        }
      },
      {
        "id": 50,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 19.35083692583329,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 37,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 18,
          "ALIGN_HEADING": 28,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 104,
          "MOTION": 5
        },
        "reasonCounts": {
          "LINK_ALIGN": 18,
          "ALIGN_HEADING": 28,
          "": 46,
          "ENTRY_POSE_SETTLE": 2,
          "NO_GROUND_PATH": 2,
          "RECOVERY_REPLAN_WAIT": 6,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "SUPPORT_BYPASS_PLANNED": 1,
          "NATIVE_MOTION": 5
        }
      },
      {
        "id": 51,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.484316320099143,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 47,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 18,
          "ALIGN_HEADING": 25
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 100,
          "MOTION": 9
        },
        "reasonCounts": {
          "": 46,
          "LINK_ALIGN": 18,
          "ALIGN_HEADING": 25,
          "RECOVERY_REPLAN_WAIT": 6,
          "NO_GROUND_PATH": 2,
          "SUPPORT_BYPASS_PLANNED": 2,
          "ENTRY_POSE_SETTLE": 1,
          "NATIVE_MOTION": 9
        }
      },
      {
        "id": 53,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.951391303132212,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 51,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 26,
          "LINK_ALIGN": 9,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "": 60,
          "RECOVERY_REPLAN_WAIT": 10,
          "ALIGN_HEADING": 26,
          "LINK_ALIGN": 9,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "NO_GROUND_PATH": 1,
          "SUPPORT_BYPASS_PLANNED": 2
        }
      },
      {
        "id": 54,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 19.36530262978205,
        "distinctRootSamples": 105,
        "nonzeroMeasuredSpeedSamples": 44,
        "longestUnchangedSampleSpanSeconds": 2.320999999999998,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 10,
          "ALIGN_HEADING": 25
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 99,
          "MOTION": 6,
          "SETTLE": 4
        },
        "reasonCounts": {
          "NO_GROUND_PATH": 2,
          "SUPPORT_BYPASS_PLANNED": 2,
          "RECOVERY_REPLAN_WAIT": 11,
          "": 48,
          "LINK_ALIGN": 10,
          "ALIGN_HEADING": 25,
          "NATIVE_MOTION": 6,
          "SETTLE": 4,
          "SUPPORTED_EGRESS_HANDOFF": 1
        }
      },
      {
        "id": 55,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.900218483368487,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 49,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 16,
          "ALIGN_HEADING": 23,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "": 57,
          "LINK_ALIGN": 16,
          "RECOVERY_REPLAN_WAIT": 7,
          "SUPPORT_BYPASS_PLANNED": 3,
          "ALIGN_HEADING": 23,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "NO_GROUND_PATH": 1,
          "ENTRY_POSE_SETTLE": 1
        }
      },
      {
        "id": 56,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.64934525441198,
        "distinctRootSamples": 108,
        "nonzeroMeasuredSpeedSamples": 52,
        "longestUnchangedSampleSpanSeconds": 0.5870000000000033,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 20,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 101,
          "MOTION": 6,
          "SETTLE": 2
        },
        "reasonCounts": {
          "": 55,
          "LINK_ALIGN": 11,
          "ALIGN_HEADING": 20,
          "SUPPORT_BYPASS_PLANNED": 3,
          "RECOVERY_REPLAN_WAIT": 5,
          "ENTRY_POSE_SETTLE": 5,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "NO_GROUND_PATH": 1,
          "NATIVE_MOTION": 6,
          "SETTLE": 2
        }
      },
      {
        "id": 57,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 17.130197772244877,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 43,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 10,
          "ALIGN_HEADING": 28
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "": 56,
          "LINK_ALIGN": 10,
          "ALIGN_HEADING": 28,
          "NO_GROUND_PATH": 3,
          "SUPPORT_BYPASS_PLANNED": 3,
          "RECOVERY_REPLAN_WAIT": 6,
          "ENTRY_POSE_SETTLE": 2,
          "EXIT_SPACE_BLOCKED": 1
        }
      },
      {
        "id": 58,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 18.953166103749357,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 46,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "LINK_ALIGN": 14,
          "ALIGN_HEADING": 22,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "LINK_ALIGN": 14,
          "ALIGN_HEADING": 22,
          "": 57,
          "RECOVERY_REPLAN_WAIT": 6,
          "NO_GROUND_PATH": 2,
          "ENTRY_POSE_SETTLE": 4,
          "SUPPORT_BYPASS_PLANNED": 2,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        }
      },
      {
        "id": 59,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 20.057301218875338,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 51,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 24,
          "LINK_ALIGN": 10,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "": 65,
          "ALIGN_HEADING": 24,
          "RECOVERY_REPLAN_WAIT": 8,
          "LINK_ALIGN": 10,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 1,
          "SUPPORT_BYPASS_PLANNED": 1
        }
      },
      {
        "id": 61,
        "observedGroup": "no_high_climb_motion_observed",
        "samples": 109,
        "highClimbStarts": 0,
        "highClimbMotionSamples": 0,
        "fallTransitions": 0,
        "pressureSupportLossTransitions": 0,
        "sampledPathTravelMeters": 15.333294162399964,
        "distinctRootSamples": 109,
        "nonzeroMeasuredSpeedSamples": 37,
        "longestUnchangedSampleSpanSeconds": 0,
        "alignmentReasonSamples": {
          "ALIGN_HEADING": 20,
          "LINK_ALIGN": 16,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        },
        "finalArrived": true,
        "phaseCounts": {
          "FOLLOW": 109
        },
        "reasonCounts": {
          "ENTRY_POSE_SETTLE": 4,
          "": 50,
          "SUPPORT_BYPASS_PLANNED": 5,
          "RECOVERY_REPLAN_WAIT": 10,
          "NO_GROUND_PATH": 2,
          "ALIGN_HEADING": 20,
          "LINK_ALIGN": 16,
          "ENTRY_ALIGNMENT_NO_PROGRESS": 2
        }
      }
    ],
    "missingWithSampledTravelOverHalfMeter": 24,
    "missingWithPressureSupportLoss": 5,
    "limitations": [
      "Activity groups describe sampled coverage, not exclusive physical root causes.",
      "Sampled polyline omits between-sample travel and includes normal movement plus displacement.",
      "Unchanged root samples can include legitimate turning; do not classify them as stopped pathfinding.",
      "Same-phase Backoff calls are not fully logged by the transition-only contract; reason samples do not establish their exact count.",
      "Final arrival does not erase missed intermediate high-stage coverage."
    ],
    "causalRootCauseEstablished": false,
    "fullGoalComplete": false
  },
  "examples": {
    "sourceRunId": "runtime-20261029-130645",
    "selection": "All TRANSITION rows for actor44 seconds98..116 and actor17 seconds50..100; not complete actor histories",
    "sourceSha256": "85e41726ebc47a567ebfeb479dd642111f5e1d33946da1cfbd63d98819b24280",
    "actor44": [
      {
        "seconds": "99.237",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "FOLLOW",
        "next": "MOTION",
        "reason": "",
        "failure": "",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "52.0495",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "2.13333",
        "link": "low_to_high_lane_-2",
        "callContext": "FOLLOW",
        "areaAtCall": "low"
      },
      {
        "seconds": "107.686",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "MOTION",
        "next": "SETTLE",
        "reason": "NATIVE_MOTION",
        "failure": "",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "54.1995",
        "motionElapsedBeforeTransition": "2.15",
        "clipDuration": "2.13333",
        "link": "low_to_high_lane_-2",
        "callContext": "MOTION",
        "areaAtCall": "high"
      },
      {
        "seconds": "109.218",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "SETTLE",
        "next": "EGRESS",
        "reason": "SETTLE",
        "failure": "",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "54.7495",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "2.13333",
        "link": "low_to_high_lane_-2",
        "callContext": "OTHER",
        "areaAtCall": "high"
      },
      {
        "seconds": "109.311",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "SUPPORTED_EGRESS_HANDOFF",
        "failure": "",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "54.7994",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "2.13333",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "high"
      },
      {
        "seconds": "110.928",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "FOLLOW",
        "next": "MOTION",
        "reason": "",
        "failure": "",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "55.3494",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "0.75",
        "link": "high_to_ground_lane_-2",
        "callContext": "FOLLOW",
        "areaAtCall": "high"
      },
      {
        "seconds": "112.824",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "MOTION",
        "next": "FALL",
        "reason": "NATIVE_MOTION",
        "failure": "",
        "recoveryGeneration": "2",
        "detail": "CLEAR",
        "nativeSeconds": "56.0994",
        "motionElapsedBeforeTransition": "0.75",
        "clipDuration": "0.75",
        "link": "high_to_ground_lane_-2",
        "callContext": "MOTION",
        "areaAtCall": "air"
      },
      {
        "seconds": "113.491",
        "id": "44",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "NATIVE_FALL",
        "failure": "",
        "recoveryGeneration": "2",
        "detail": "CLEAR",
        "nativeSeconds": "56.3494",
        "motionElapsedBeforeTransition": "0",
        "clipDuration": "0.75",
        "link": "high_to_ground_lane_-2",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      }
    ],
    "actor17": [
      {
        "seconds": "51.462",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "FOLLOW",
        "next": "FALL",
        "reason": "ALIGN_HEADING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "28.3999",
        "link": "none",
        "callContext": "PRESSURE_SUPPORT_LOSS",
        "areaAtCall": "low"
      },
      {
        "seconds": "91.756",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "FALL",
        "next": "LAND",
        "reason": "NATIVE_FALL",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "48.2495",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      {
        "seconds": "97.711",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "LAND",
        "next": "EGRESS",
        "reason": "LAND",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "51.3495",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      },
      {
        "seconds": "97.803",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "ARRIVED",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "detail": "CLEAR",
        "nativeSeconds": "51.3995",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "ground"
      }
    ]
  },
  "costs": [
    {
      "runId": "runtime-20261029-130025",
      "stages": {
        "LOW_HOLD": {
          "seconds": 39.414,
          "intervals": 77,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 4.6985264799978905,
            "perceptionCpuTicks": 2.1162542583565402,
            "socialCpuTicks": 2.1072396792958523,
            "pursuitCpuTicks": 0.15219237966658286,
            "crowdCpuTicks": 6.474637049305157,
            "staticCpuTicks": 0.8579047779641631,
            "commitTicks": 6.915103133693205,
            "traceTicks": 13.04328146797609,
            "missionUpdateTicks": null,
            "driveTicks": 24.222978220212227,
            "navTicks": 0.06048115272971579,
            "bypassTicks": 0.0,
            "contactCpuTicks": 34.84409836612585,
            "contactBoneCpuTicks": 1.2182367601701771,
            "contactFootCpuTicks": 20.655894316571658,
            "contactWorldCpuTicks": 11.088345804000822,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.1651167437913069
          },
          "missionUpdatesPerSecond": null
        },
        "HIGH_HOLD": {
          "seconds": 59.405,
          "intervals": 116,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 4.360330802829728,
            "perceptionCpuTicks": 2.0480012118610498,
            "socialCpuTicks": 1.8784141812387256,
            "pursuitCpuTicks": 0.14921497256775595,
            "crowdCpuTicks": 6.4311196985351575,
            "staticCpuTicks": 0.7522595382058667,
            "commitTicks": 6.803075569278621,
            "traceTicks": 12.861582885531622,
            "missionUpdateTicks": null,
            "driveTicks": 23.003053296330496,
            "navTicks": 0.0,
            "bypassTicks": 0.0,
            "contactCpuTicks": 35.649451899833075,
            "contactBoneCpuTicks": 1.253563332338437,
            "contactFootCpuTicks": 21.39394351499012,
            "contactWorldCpuTicks": 11.171398979251228,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.06361596809950322
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_HOLD": {
          "seconds": 29.730000000000018,
          "intervals": 58,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 6.660489237673338,
            "perceptionCpuTicks": 2.0390213800286734,
            "socialCpuTicks": 2.3233811086126788,
            "pursuitCpuTicks": 1.9137604823179049,
            "crowdCpuTicks": 5.6805009845038335,
            "staticCpuTicks": 1.0879467325713532,
            "commitTicks": 6.543906458010201,
            "traceTicks": 13.54526409992654,
            "missionUpdateTicks": null,
            "driveTicks": 14.215800081630126,
            "navTicks": 0.007130856690301529,
            "bypassTicks": 0.0,
            "contactCpuTicks": 17.35630336749514,
            "contactBoneCpuTicks": 0.7305899040378553,
            "contactFootCpuTicks": 9.22625220152334,
            "contactWorldCpuTicks": 6.349422432617166,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_EAST": {
          "seconds": 9.152999999999992,
          "intervals": 18,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 25.66190421451989,
            "perceptionCpuTicks": 2.0018608027087863,
            "socialCpuTicks": 12.217982613530879,
            "pursuitCpuTicks": 11.038411018130253,
            "crowdCpuTicks": 6.388408378365491,
            "staticCpuTicks": 1.0646801027341115,
            "commitTicks": 7.15340479492211,
            "traceTicks": 14.245627357157598,
            "missionUpdateTicks": null,
            "driveTicks": 21.230234742256805,
            "navTicks": 0.3142909442314232,
            "bypassTicks": 0.0,
            "contactCpuTicks": 22.571327431950106,
            "contactBoneCpuTicks": 1.2305495057070135,
            "contactFootCpuTicks": 11.937199867094074,
            "contactWorldCpuTicks": 7.900046541672742,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_EAST_HOLD": {
          "seconds": 14.432999999999993,
          "intervals": 28,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 7.003060771820846,
            "perceptionCpuTicks": 2.0782964225727243,
            "socialCpuTicks": 1.6834367308724323,
            "pursuitCpuTicks": 2.82769147879844,
            "crowdCpuTicks": 6.3311272238107685,
            "staticCpuTicks": 1.0063760347335917,
            "commitTicks": 6.739289786331634,
            "traceTicks": 13.07977059118812,
            "missionUpdateTicks": null,
            "driveTicks": 15.082822650681967,
            "navTicks": 0.01179936927470779,
            "bypassTicks": 0.0,
            "contactCpuTicks": 15.141299853664078,
            "contactBoneCpuTicks": 0.8476769429912653,
            "contactFootCpuTicks": 7.718367221568839,
            "contactWorldCpuTicks": 5.416554854896248,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_NORTH": {
          "seconds": 9.745000000000005,
          "intervals": 19,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 24.779930299696716,
            "perceptionCpuTicks": 2.0154986376360076,
            "socialCpuTicks": 12.562156316996322,
            "pursuitCpuTicks": 9.800222309971572,
            "crowdCpuTicks": 6.581950924933107,
            "staticCpuTicks": 1.0225774616385528,
            "commitTicks": 7.062095424983964,
            "traceTicks": 14.047228170968538,
            "missionUpdateTicks": null,
            "driveTicks": 16.062008490267218,
            "navTicks": 0.23820462234837655,
            "bypassTicks": 0.0,
            "contactCpuTicks": 17.366166740138205,
            "contactBoneCpuTicks": 1.1404432391034842,
            "contactFootCpuTicks": 8.775182042605039,
            "contactWorldCpuTicks": 6.023612343018871,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "FINAL_HOLD": {
          "seconds": 129.177,
          "intervals": 252,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 5.54163084251729,
            "perceptionCpuTicks": 2.0680884793569203,
            "socialCpuTicks": 1.6681017167641623,
            "pursuitCpuTicks": 1.3881520119964696,
            "crowdCpuTicks": 6.103651376381585,
            "staticCpuTicks": 1.20971436315939,
            "commitTicks": 6.292075340489807,
            "traceTicks": 12.991886514569593,
            "missionUpdateTicks": null,
            "driveTicks": 11.274054496021073,
            "navTicks": 0.0,
            "bypassTicks": 0.0,
            "contactCpuTicks": 9.391355379542695,
            "contactBoneCpuTicks": 0.508276352647313,
            "contactFootCpuTicks": 4.5600379820013295,
            "contactWorldCpuTicks": 3.4986010472942364,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        }
      },
      "calibration": {
        "ticksPerSecond": 9999982.57537637,
        "ticks": 2941234875,
        "seconds": 294.124,
        "pairs": 574,
        "unsignedWraps": 1,
        "intervalRateMinimum": 9730912.350597976,
        "intervalRateMaximum": 10274870.333987918,
        "method": "Sum paired actor0 unsigned32 TickCount deltas / same foreground GetTickTime interval sum. Interval variation retained; not CPU self-time."
      },
      "limitations": [
        "Elapsed scopes overlap; do not sum or call their remainder renderer cost.",
        "senseTicks includes perception, social/intent, pursuit and yield processing.",
        "navTicks and bypassTicks can nest within social/intent or native drive.",
        "missionUpdateTicks measures its instrumented OnUpdate scope, not a whole engine frame.",
        "No AI-call counter: mission update frequency is not AI frequency.",
        "No native graph/render upload/GPU/allocation/GC self-time.",
        "Concurrent Workbench and instrumentation overhead are not removed."
      ]
    },
    {
      "runId": "runtime-20261029-130645",
      "stages": {
        "LOW_HOLD": {
          "seconds": 39.452,
          "intervals": 73,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 53.618820461863244,
            "perceptionCpuTicks": 50.28856582894676,
            "socialCpuTicks": 2.502679982567737,
            "pursuitCpuTicks": 0.3966419666217872,
            "crowdCpuTicks": 12.385051760906695,
            "staticCpuTicks": 2.02374029489355,
            "commitTicks": 9.397651076199368,
            "traceTicks": 57.157632604800725,
            "missionUpdateTicks": null,
            "driveTicks": 249.66492262498932,
            "navTicks": 1.9388853429794273,
            "bypassTicks": 0.0,
            "contactCpuTicks": 239.91584618851505,
            "contactBoneCpuTicks": 6.273609613812224,
            "contactFootCpuTicks": 135.1971511617969,
            "contactWorldCpuTicks": 89.10727962148147,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.7560406155894414
          },
          "missionUpdatesPerSecond": null
        },
        "HIGH_HOLD": {
          "seconds": 59.675,
          "intervals": 109,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 54.05466656306144,
            "perceptionCpuTicks": 50.08664755079765,
            "socialCpuTicks": 3.216756160929985,
            "pursuitCpuTicks": 0.4066473871296228,
            "crowdCpuTicks": 12.28279659321908,
            "staticCpuTicks": 1.946913002064873,
            "commitTicks": 8.349561836801332,
            "traceTicks": 62.2410305906429,
            "missionUpdateTicks": null,
            "driveTicks": 231.47942073360207,
            "navTicks": 2.6813232500605513,
            "bypassTicks": 0.0,
            "contactCpuTicks": 225.67051702900062,
            "contactBoneCpuTicks": 5.900728443191529,
            "contactFootCpuTicks": 129.52078516323832,
            "contactWorldCpuTicks": 79.92376957757062,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 1.9102303569185632
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_HOLD": {
          "seconds": 29.187999999999988,
          "intervals": 52,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 112.81602553815588,
            "perceptionCpuTicks": 71.48448597639774,
            "socialCpuTicks": 5.323637129283998,
            "pursuitCpuTicks": 35.52568343837619,
            "crowdCpuTicks": 11.906544557930824,
            "staticCpuTicks": 4.773682684616676,
            "commitTicks": 10.755935993866663,
            "traceTicks": 105.50465285064207,
            "missionUpdateTicks": null,
            "driveTicks": 134.3835986550177,
            "navTicks": 0.5269340518171647,
            "bypassTicks": 0.0,
            "contactCpuTicks": 199.0592553413895,
            "contactBoneCpuTicks": 6.853554624706289,
            "contactFootCpuTicks": 110.35047980213737,
            "contactWorldCpuTicks": 71.88873709233059,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.9179655701296449
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_EAST": {
          "seconds": 9.495999999999981,
          "intervals": 17,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 264.09314723799463,
            "perceptionCpuTicks": 80.03355499281432,
            "socialCpuTicks": 125.21325438263234,
            "pursuitCpuTicks": 58.34808267931182,
            "crowdCpuTicks": 10.441036341864077,
            "staticCpuTicks": 3.94076468465717,
            "commitTicks": 9.376505508703065,
            "traceTicks": 106.13088110547072,
            "missionUpdateTicks": null,
            "driveTicks": 139.4168965397462,
            "navTicks": 3.247524516569587,
            "bypassTicks": 0.0,
            "contactCpuTicks": 162.06602722961762,
            "contactBoneCpuTicks": 6.1927370177990575,
            "contactFootCpuTicks": 89.81144443188431,
            "contactWorldCpuTicks": 58.270901561536654,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_EAST_HOLD": {
          "seconds": 14.431999999999988,
          "intervals": 26,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 126.155843284849,
            "perceptionCpuTicks": 83.67411533330258,
            "socialCpuTicks": 2.16922370984053,
            "pursuitCpuTicks": 39.73815532587777,
            "crowdCpuTicks": 15.649449108233119,
            "staticCpuTicks": 3.0479290535616244,
            "commitTicks": 12.541452441882237,
            "traceTicks": 96.98113538916559,
            "missionUpdateTicks": null,
            "driveTicks": 157.94269559228957,
            "navTicks": 0.005701919876073959,
            "bypassTicks": 0.0,
            "contactCpuTicks": 182.98839596847867,
            "contactBoneCpuTicks": 8.29465836125413,
            "contactFootCpuTicks": 98.3622055083595,
            "contactWorldCpuTicks": 66.56620795883333,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "GROUND_NORTH": {
          "seconds": 9.710000000000008,
          "intervals": 18,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 273.0979630456271,
            "perceptionCpuTicks": 86.16986782406819,
            "socialCpuTicks": 137.66522220733322,
            "pursuitCpuTicks": 48.697544475663626,
            "crowdCpuTicks": 13.265140680795476,
            "staticCpuTicks": 3.444385115913118,
            "commitTicks": 20.56601379264456,
            "traceTicks": 108.73049250777784,
            "missionUpdateTicks": null,
            "driveTicks": 117.3059806743303,
            "navTicks": 3.115094113979818,
            "bypassTicks": 0.0,
            "contactCpuTicks": 131.8354389865402,
            "contactBoneCpuTicks": 6.18408666389511,
            "contactFootCpuTicks": 71.17558382459612,
            "contactWorldCpuTicks": 47.090631571426606,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        },
        "FINAL_HOLD": {
          "seconds": 129.19199999999998,
          "intervals": 240,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 115.36634075604363,
            "perceptionCpuTicks": 87.1876426264949,
            "socialCpuTicks": 3.28247121610157,
            "pursuitCpuTicks": 23.947984969193726,
            "crowdCpuTicks": 17.2243830539903,
            "staticCpuTicks": 5.160658957398054,
            "commitTicks": 18.845031626685927,
            "traceTicks": 102.09234664415123,
            "missionUpdateTicks": null,
            "driveTicks": 143.8408343383137,
            "navTicks": 0.00015401579081491382,
            "bypassTicks": 0.0,
            "contactCpuTicks": 144.77717295159863,
            "contactBoneCpuTicks": 5.573767231849834,
            "contactFootCpuTicks": 80.60876911773563,
            "contactWorldCpuTicks": 49.69518395458467,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.0
          },
          "missionUpdatesPerSecond": null
        }
      },
      "calibration": {
        "ticksPerSecond": 10001202.1309466,
        "ticks": 2945524048,
        "seconds": 294.517,
        "pairs": 541,
        "unsignedWraps": 1,
        "intervalRateMinimum": 8649594.285714192,
        "intervalRateMaximum": 11429375.432525894,
        "method": "Sum paired actor0 unsigned32 TickCount deltas / same foreground GetTickTime interval sum. Interval variation retained; not CPU self-time."
      },
      "limitations": [
        "Elapsed scopes overlap; do not sum or call their remainder renderer cost.",
        "senseTicks includes perception, social/intent, pursuit and yield processing.",
        "navTicks and bypassTicks can nest within social/intent or native drive.",
        "missionUpdateTicks measures its instrumented OnUpdate scope, not a whole engine frame.",
        "No AI-call counter: mission update frequency is not AI frequency.",
        "No native graph/render upload/GPU/allocation/GC self-time.",
        "Concurrent Workbench and instrumentation overhead are not removed."
      ]
    }
  ]
}

Own calibration source excerpt

    """Pair actor0 only: cpuClockCounter is repeated, not an actor cost."""
    series=[r for r in rows if r['id']=='0']
    assert len(series)>=2
    ticks=0;seconds=0.;pairs=0;wraps=0;rates=[]
    for before,after in zip(series,series[1:]):
        t0=float(before['seconds']);t1=float(after['seconds']);assert t1>t0
        if t0<5 or t1-t0>1.5 or before['appActive']!='true' or after['appActive']!='true':continue
        raw=int(after['cpuClockCounter'])-int(before['cpuClockCounter'])
        delta=raw & 0xffffffff
        assert delta>0
        ticks+=delta;seconds+=t1-t0;pairs+=1;wraps+=raw<0;rates.append(delta/(t1-t0))
    assert seconds>0 and pairs>0
    return dict(ticksPerSecond=ticks/seconds,ticks=ticks,seconds=seconds,pairs=pairs,
        unsignedWraps=wraps,intervalRateMinimum=min(rates),intervalRateMaximum=max(rates),
        method='Sum paired actor0 unsigned32 TickCount deltas / same foreground GetTickTime interval sum. Interval variation retained; not CPU self-time.')


이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
