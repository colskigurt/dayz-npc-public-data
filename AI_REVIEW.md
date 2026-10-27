# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261027T224126-frame-order-20261028

네이티브 갱신 시각·시계 원점 교정 pilot와 분석기 경계 수정

작성: 2026-10-28T07:40:47.287797+09:00

1. 작업 목표

CPU 포즈·실제 표시의 시간적 불일치 조사에서 네이티브 호출과 HUD/본 조회의 실행 순서를 같은 시계로 연결한다. 기존 파이프라인과 정상 낙하/복구는 보존하고 시각·기능·성능 수용을 분리한다. 원인 특정 전 새 보간이나 강제 갱신 정책은 적용하지 않는다.

2. 실제 수행

- 기존 고정 달리기 미션에서 별도 frame-order 미션을 만들고 HUD/6본 조회 앞뒤에 TickCount(0) 읽기2개만 추가했다. 기본 미션/PBO를 바꾸지 않고 B8FB 대상으로 격리 compile pass를 확인했다.
- 기존 Python Frida 단일 helper 진입 훅에 QPC 읽기만 추가했다. 원본 바이트·두 반환PC·receiver/renderer/73본 조건·saved input dt ABI를 검증했다. 반환/내부 훅이나 native 인자/반환값·게임 상태 override는 없다. REA MCP 직접 Evidence가 아니며 read-only 프로세스 접근만 한 시험도 아니다.
- 작은 샘플에서 per-thread QPC scratch와 정상 계산24,000호출을 확인했다. 첫 샘플은 과거 non-export 빌드 참조로 실패했다. 다음 실행은 종료 후 metadata 경로 오류로 원시 메시지를 보존하지 못했고 성공으로 재구성하지 않았다. 수정한 세 번째 샘플의 저장된 결과만 수용했다.
- 동일1개체/8part, AI·이동 OFF, 달리기, 동일 그래픽/VSync로120초3회를 정상 종료했다. 각45초 창 한정 lossless 영상도 finalize했다. 첫 대기 스크립트는 헤더 숫자 변환에 실패해 고친 뒤 실행 후반에 attach했다. 이 준비 실패와 source 한계를 보존했다.
- 초기 분석기의 TickCount/QPC 원점 동일 가정은 검증되지 않았음을 확인해 해당 결과를 rejected sidecar와 함께 보존하고 도구를 사용 중지했다. 기존 앞선 clock bridge도 nativeTickMatchesSharedQpc=false였다. 같은 속도로 증가하는 시계는 같은 원점을 보장하지 않는다.
- 세 번째 실행은 외부 파일 nonce/ack만으로75초 동안 두 시계 차이를 교정했다. 해당 수집기는 대상 프로세스 메모리를 읽거나 쓰지 않는다. 모호한 시간 연결은 수용하지 않았다.

3. 측정 결과

|실행 ID|실제 호출/연속쌍|정확 모델 잔차ms|reset|입력 합계ms|초과분 소실ms|frame p50/p95/p99 ms|
|---|---|---:|---:|---:|---:|---|
|runtime-20261028-072238|399/398|0.0|101|7960.000|2952.000|20.0684/35.3299/37.2362|
|runtime-20261028-072554|400/399|0.0|101|7965.000|2909.000|20.0658/35.2539/37.1499|
|runtime-20261028-073152|400/399|0.0|104|7968.000|2722.000|20.0919/35.0849/36.7441|

총1,199개 실제 진입,1,196개 인접 호출쌍. 첫2회는 같은 네이티브 계측 조건 반복이며 세 번째는 추가 파일 왕복을 포함한 pilot이다. 세 실행 모두 정확 float32 누적기 모델과 전이 잔차0으로 일치했다. 표의 초과분은 해당 누적기에서 보존되지 않은 시간이며 전체 클립/그래프 또는 모든 끊김의 손실 확정이 아니다. 비율1이상 표본은 없었다.

세 번째1,846 ACK/timeout0으로 측정한 시계 차이 구간 폭은7.5055ms였고0은 포함되지 않았다.400호출 중205개는 HUD 종료 뒤/다음 HUD 시작 전임이 구간 전체에서 확실했고195개는 시간 경계가 모호해 제외했다. 연속·고유·좋은 FK fit 조건은10쌍으로 최소30쌍 기준 미달이다. 그10쌍의 phase proxy 잔차 RMS1.461ms는 참고 관측이고 충분한 실행순서 수용/상관 인과 증거가 아니다.

세 번째20~23초/25~28초 영상360프레임 모두 HUD 행과 정확히 연결됐다. 별도298 CPU·실루엣 연속쌍의 같은 프레임 상관0.936323. 교정 native·pose·영상3자 중첩은8쌍뿐이며 미수용이다. 첫2회 native/HUD 직접 연결도 교정 부재로 미수용이다. CPU/영상 자체의 연결과 네이티브 호출의 직접 연결은 구분한다.

8/64명 기능 회귀·64 비용 귀속은 이번에 미실행이다. 표의 frame dt는15초 이후 전경 HUD callback 시작 간격이며1개체 계측·녹화를 포함한다. GPU/Present나 native 애니메이션 FPS가 아니고20ms p95 또는 성능 개선을 달성한 결과가 아니다.

4. 원인 분석

확인: 실제 입력 pDt와 누적기 초기화는 원본 ADDSS·COMISS/JB 조건의 float32 모델과 일치한다. 초과분을 버리는 경계를 재확인했지만 그래프 전체 보상/개별 clip phase는 직접 읽지 않았다.

분석기 결함 수정: 기존 '-1e-7' 비교 오차 허용은0.03999999910593033+0.009999999776482582가 float32로0.04999999701976776이 된 경우에도 잘못 reset으로 판정했다. 실제 임계값0.05000000074505806보다 작은 값이므로 엔진은 유지하는 것이 맞다. 기계어에 없는 epsilon을 제거한 모델은 세 번째399/399쌍을 정확히 설명했다. 처음의50ms 잔차는 새 엔진 문제로 보고하지 않는다. 기존 결과는 보존하고 수정본을 별도 저장했다.

반증/기각: 원점0을 가정한 프레임 연결 결과는 사용하지 않는다. 그 결과의 다중 호출 bucket·실행순서·포즈 잔차를 실제 엔진 현상으로 해석하지 않는다. 앞선480개 endpoint 연속성 결과는 이 가정을 사용하지 않아 그대로 보존한다.

추정: native50ms 갱신과 실제 벽시계 간격의 차이가 시각적 불균등에 기여할 수 있으나, 이번 교정 정확도는 단독 원인/실행 순서를 충분히 판정하기에 부족하다. 이전 평균-rate 후보가 부드러움을 해결하지 못했다는 기록도 유지한다. 부족한 표본을 이유로 합격선을 낮추거나 업데이트 정책을 변경하지 않는다.

5. 검증 수준

진단 성공: 단일 진입1,199호출 포착, byte/ABI guard·무오류/누락0·정상 종료, 서로 다른 시계 원점의 실제 교정.
반복 검증 완료: 첫2회 같은 네이티브 조건의 누적기 전이; 추가 pilot 포함3회 정확 float32 모델 잔차0.
회귀 통과: 오프라인 시계 원점/low32 wrap/잘못된 ACK/충돌 구간/실제 float32 경계7검사. 게임 기능 회귀를 뜻하지 않는다.
미수용/미실행: native-HUD/영상 직접 연결 표본 충분성, 지각적 부드러움, 개별 clip phase/GPU/Present,8/64 새 회귀·할당/GC·64 비용 귀속.20ms p95는 목표다.
최종 수용 미완료. 골은 active로 유지한다.

6. 변경 사항

별도 frame-order 미션과 launcher, entry-QPC 관측, nonce/ack clock 수집기, calibrated analyzer,7개 회귀 검사를 추가했다. 기존 sample probe는 명시적 frameOrder launch일 때만 그 별도 미션을 허용하도록 범위를 확장했다. 컴파일 helper는 그 격리 이름만 추가했다. 기본 경로/동작은 유지했다.

기존 analyze_horde_frame_order.py는 잘못된 원점 가정을 가진 버전을 보관하고 사용 중지 오류를 내도록 했다. 분석 driver의 archived JS는 과거 LF 실행 text와 CRLF 파일을 구분해 해시 검증하고 미래 archive는LF로 저장한다. 실제 실행 snapshot은 수정하지 않았다.

PBO/그래프/모델/AI 코드 재빌드·재서명 없음. B8FB integration/453D base/0C6E assets 유지, 실행마다 기존3개PBO 서명 검증 성공. 운영 서버 미변경. 원시 로그/영상/기존 보고서 미덮어쓰기.

7. 원시 증거

runtime-20261028-072238/072554/073152의 launch, engine compile, mission source, observer driver/실행 JS/configuration/status/calls, HUD/pose TSV, clock bridge, 녹화/실제 창 확인과 분석 JSON을 보존했다. 잘못된 초기 연결은 rejected sidecar, 잘못된 epsilon 모델은 model-correction sidecar로 범위를 명시했다.

QPC 보조 샘플 metadata 실패 실행의 원시 메시지는 저장되지 않았다. 이를 복구/검증한 것처럼 주장하지 않는다. 이후 저장된 exported fixture 결과는 별개다. 전체 파일 SHA·크기·저장 전 두 번 검사 결과는 아래 로컬 목록에 있다. 공개판은 요약·일부 값·해시만 제공하고 내부 경로·프로세스 정보·원본 자산/영상·전체 메모리/로그를 노출하지 않는다.

8. 잔여 문제

다음은 TickCount native 구현/원점 저장 위치를 고정 실행파일에서 확인해, 가능한 경우 비변경 읽기 근거로 두 시계의 정확한 관계를 확보하는 것이다. 또는 기존 nonce/ack 교정의 불확실성을 실제로 줄일 수 있는 방법을 검토한다. 같은75초 수집을 근거 없이 반복하거나10쌍으로 수용하지 않는다.

지원되는 VSync/시간 정책 대조가 어느 수준까지 이미 반복됐는지도 확인해, 원인 후보를 바꾸기 전에 재현 가능한 비교 조건을 정한다. 원인 특정 전 보간·강제 업데이트를 적용하지 않는다.64 기능 계약과 비용 실험은 별도 상태로 유지하고 기존 고지대13/14/43 미도달·EGRESS_STALL1·최종64/64를 이번 단일 애니메이션 결과로 대체하지 않는다.

원본 보고서 ID: 2026-10-28_074047_287797+0900_npc-frame-clock-pilot.md
원본 SHA-256: b006250af74b6ab1f17ff26950db6107c1a91618819fb1243645b3cffeee1784
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 결과 (원시 전체 아님)
[
  {
    "summary": {
      "runId": "runtime-20261028-072238",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "terminalArrivals": null,
      "missingHighFollow": null,
      "fallsLandedAndResumed": null,
      "admissionOutcomes": null,
      "egressViolations": null,
      "climbEndFailures": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.0684,
        "p95": 35.3299,
        "p99": 37.2362
      },
      "nativeCalls": 399,
      "nativeAdjacentPairs": 398,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 101,
      "discardedOvershootSumMs": 2951.9999511539936,
      "inputSumMs": 7960.000121500343,
      "clockCalibrationMeasured": false,
      "clockOffsetWidthMs": null,
      "definiteFrameEntries": null,
      "uniquePosePairs": null,
      "frameJoinAdmitted": false,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "admissionReapproach": false,
        "frameOrderMission": true,
        "clockExchange": false,
        "performanceAcceptance": false,
        "repetitions": 1,
        "comparableNativeOnlyRepeats": 2,
        "frameWindow": "t>=15s consecutive HUD callback begin ticks, foreground; not GPU/Present"
      }
    },
    "nativeModel": {
      "adjacentCalls": 398,
      "thresholdCrossings": 101,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 101,
      "maxResidualMs": 0.0,
      "observedInputSumMs": 7960.000121500343,
      "consumedThresholdsMs": 5050.000075250864,
      "retainedAccumulatorDeltaMs": -41.999999433755875,
      "thresholdOvershootSumMs": 2951.9999511539936,
      "thresholdOvershootMs": {
        "p05": 26.999998837709427,
        "p50": 29.999997466802597,
        "p95": 32.00000151991844,
        "p99": 33.000003546476364
      },
      "conservationResidualMs": 9.452924132347107e-05,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "clock": {
      "clockCalibrationMeasured": false,
      "frameJoinAdmitted": false
    },
    "sourceHashes": {
      "native-call-probe/analysis-float32-exact.json": "eda0ee0e88eb578f53faa10bb73346d815eb873343a561a23965e08cab167369",
      "native-call-probe/calls.jsonl": "7b9acf2a8f09bc1215d2817b0da5bb9b8801b7376cac73889dccb5728677bf72",
      "visible-frame-clock.tsv": "fd88a177ba633ee1395c1487bce8ff3fd8a14c412e002e6bcac90aeb549a45e3"
    }
  },
  {
    "summary": {
      "runId": "runtime-20261028-072554",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "terminalArrivals": null,
      "missingHighFollow": null,
      "fallsLandedAndResumed": null,
      "admissionOutcomes": null,
      "egressViolations": null,
      "climbEndFailures": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.0658,
        "p95": 35.2539,
        "p99": 37.1499
      },
      "nativeCalls": 400,
      "nativeAdjacentPairs": 399,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 101,
      "discardedOvershootSumMs": 2909.00007635355,
      "inputSumMs": 7965.000186115503,
      "clockCalibrationMeasured": false,
      "clockOffsetWidthMs": null,
      "definiteFrameEntries": null,
      "uniquePosePairs": null,
      "frameJoinAdmitted": false,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "admissionReapproach": false,
        "frameOrderMission": true,
        "clockExchange": false,
        "performanceAcceptance": false,
        "repetitions": 1,
        "comparableNativeOnlyRepeats": 2,
        "frameWindow": "t>=15s consecutive HUD callback begin ticks, foreground; not GPU/Present"
      }
    },
    "nativeModel": {
      "adjacentCalls": 399,
      "thresholdCrossings": 101,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 101,
      "maxResidualMs": 0.0,
      "observedInputSumMs": 7965.000186115503,
      "consumedThresholdsMs": 5050.000075250864,
      "retainedAccumulatorDeltaMs": 6.000000052154064,
      "thresholdOvershootSumMs": 2909.00007635355,
      "thresholdOvershootMs": {
        "p05": 22.999998182058334,
        "p50": 29.999997466802597,
        "p95": 33.000003546476364,
        "p99": 33.000003546476364
      },
      "conservationResidualMs": 3.4458935260772705e-05,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "clock": {
      "clockCalibrationMeasured": false,
      "frameJoinAdmitted": false
    },
    "sourceHashes": {
      "native-call-probe/analysis-float32-exact.json": "996933069ebeb51ebce1e09297420e8a3758c2d91f4e06ba877cc8341198b33b",
      "native-call-probe/calls.jsonl": "495cd00ab3c2e9a5dd3f43edb7c7e5dc8909a80394c15aa4f94c8bc7e725e2a2",
      "visible-frame-clock.tsv": "9f0b6cae101ab6330a22b76e273ad1ebc879b14f1e6eca14928c34b27aadf208"
    }
  },
  {
    "summary": {
      "runId": "runtime-20261028-073152",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "terminalArrivals": null,
      "missingHighFollow": null,
      "fallsLandedAndResumed": null,
      "admissionOutcomes": null,
      "egressViolations": null,
      "climbEndFailures": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.0919,
        "p95": 35.0849,
        "p99": 36.7441
      },
      "nativeCalls": 400,
      "nativeAdjacentPairs": 399,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 104,
      "discardedOvershootSumMs": 2721.999943256378,
      "inputSumMs": 7968.000095337629,
      "clockCalibrationMeasured": true,
      "clockOffsetWidthMs": 7.5055,
      "definiteFrameEntries": 205,
      "uniquePosePairs": 10,
      "frameJoinAdmitted": false,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "admissionReapproach": false,
        "frameOrderMission": true,
        "clockExchange": true,
        "performanceAcceptance": false,
        "repetitions": 1,
        "comparableNativeOnlyRepeats": 0,
        "frameWindow": "t>=15s consecutive HUD callback begin ticks, foreground; not GPU/Present"
      }
    },
    "nativeModel": {
      "adjacentCalls": 399,
      "thresholdCrossings": 104,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 104,
      "maxResidualMs": 0.0,
      "observedInputSumMs": 7968.000095337629,
      "consumedThresholdsMs": 5200.000077486038,
      "retainedAccumulatorDeltaMs": 46.000003814697266,
      "thresholdOvershootSumMs": 2721.999943256378,
      "thresholdOvershootMs": {
        "p05": 1.149998046457768,
        "p50": 29.999997466802597,
        "p95": 32.00000151991844,
        "p99": 33.96999828517437
      },
      "conservationResidualMs": 7.078051567077637e-05,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "clock": {
      "clockCalibrationMeasured": true,
      "frameJoinAdmitted": false
    },
    "sourceHashes": {
      "native-call-probe/analysis-float32-exact.json": "00e6bfca5f27ddd42cab5df8bef59281b3e66d7f5b4beb64fed28809edfaa0d9",
      "native-call-probe/calls.jsonl": "d8656d7722c398d32efd7bcbb8fd61d919a9fafda40cbf62964ccf5e19a10faf",
      "visible-frame-clock.tsv": "84f76bb5c72e82ddbb4a8bc9ac027ee2f66a0a74d31fa52c82308f96c1b9fdee"
    },
    "calibration": {
      "clockOffsetWidthMs": 7.5055,
      "clockAcks": 1846,
      "clockOriginIsZero": false,
      "frameJoinCounts": {
        "clockOrHUDOverlapAmbiguous": 195,
        "definitelyAfterHUDBeforeNext": 205,
        "excludedNonuniqueHUDPairs": 194
      },
      "uniqueConsecutivePosePairs": 10,
      "sourceProxyResidualRmsMs": 1.4605939301338546
    },
    "videoJoin": {
      "runId": "runtime-20261028-073152",
      "calibratedNativePosePairs": 10,
      "exactVideoHudFrames": 360,
      "cpuVisible": {
        "pairs": 298,
        "correlation": 0.9363230146571965,
        "queriedRmsChangeMetersP50": 0.022969238000711625,
        "queriedRmsChangeMetersP95": 0.11201594080977939,
        "visibleMaskChangePixelsP50": 1288,
        "visibleMaskChangePixelsP95": 4182
      },
      "calibratedNativePoseVisiblePairs": 8,
      "selectedPairs": [
        {
          "firstFrame": 1221,
          "lastFrame": 1222,
          "reset": false,
          "inputDtMs": 28.00000086426735,
          "modelPoseDeltaMs": 28.0000027269125,
          "sourceProxyDeltaMs": 29.166666666666668,
          "residualMs": 1.1666639397541687,
          "callbackDeltaMs": 0.0,
          "callbackResetResidualMs": 0.0
        },
        {
          "firstFrame": 1222,
          "lastFrame": 1223,
          "reset": true,
          "inputDtMs": 19.999999552965164,
          "modelPoseDeltaMs": 11.99999824166298,
          "sourceProxyDeltaMs": 12.5,
          "residualMs": 0.5000017583370215,
          "callbackDeltaMs": 50.00000000000071,
          "callbackResetResidualMs": 7.45057349149647e-07
        },
        {
          "firstFrame": 1269,
          "lastFrame": 1270,
          "reset": false,
          "inputDtMs": 28.00000086426735,
          "modelPoseDeltaMs": 28.0000027269125,
          "sourceProxyDeltaMs": 29.166666666666668,
          "residualMs": 1.1666639397541687,
          "callbackDeltaMs": 0.0,
          "callbackResetResidualMs": 0.0
        },
        {
          "firstFrame": 1270,
          "lastFrame": 1271,
          "reset": true,
          "inputDtMs": 12.000000104308128,
          "modelPoseDeltaMs": 11.99999824166298,
          "sourceProxyDeltaMs": 12.5,
          "residualMs": 0.5000017583370215,
          "callbackDeltaMs": 49.99999999999716,
          "callbackResetResidualMs": 7.450609018633259e-07
        },
        {
          "firstFrame": 1273,
          "lastFrame": 1274,
          "reset": true,
          "inputDtMs": 28.999999165534973,
          "modelPoseDeltaMs": 12.000001966953278,
          "sourceProxyDeltaMs": 8.333333333333334,
          "residualMs": -3.6666686336199446,
          "callbackDeltaMs": 50.00000000000071,
          "callbackResetResidualMs": 7.45057349149647e-07
        },
        {
          "firstFrame": 1521,
          "lastFrame": 1522,
          "reset": true,
          "inputDtMs": 32.00000151991844,
          "modelPoseDeltaMs": 4.0000006556510925,
          "sourceProxyDeltaMs": 4.166666666666667,
          "residualMs": 0.16666601101557407,
          "callbackDeltaMs": 50.00000000000071,
          "callbackResetResidualMs": 7.45057349149647e-07
        },
        {
          "firstFrame": 1522,
          "lastFrame": 1523,
          "reset": false,
          "inputDtMs": 12.000000104308128,
          "modelPoseDeltaMs": 12.000000104308128,
          "sourceProxyDeltaMs": 12.5,
          "residualMs": 0.4999998956918723,
          "callbackDeltaMs": 0.0,
          "callbackResetResidualMs": 0.0
        },
        {
          "firstFrame": 1523,
          "lastFrame": 1524,
          "reset": false,
          "inputDtMs": 28.00000086426735,
          "modelPoseDeltaMs": 27.9999990016222,
          "sourceProxyDeltaMs": 29.166666666666668,
          "residualMs": 1.1666676650444672,
          "callbackDeltaMs": 0.0,
          "callbackResetResidualMs": 0.0
        }
      ],
      "accepted": false,
      "reason": "Only10 consecutive calibrated pose pairs /8 video triples; minimum30 pair gate not met. No causality or perceptual smoothness acceptance."
    }
  }
]

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
