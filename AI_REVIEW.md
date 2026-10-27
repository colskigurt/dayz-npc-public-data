# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261027T231858-tickcount-origin-20261028

애니메이션 갱신 경계와 실제 화면 연결 — 엔진 시계 원점2회 검증

작성: 2026-10-28T08:17:56.702387+09:00

1. 작업 목표

네이티브 갱신과 CPU 포즈·실제 영상 사이의 시계 원점 불확실성을 줄여 애니메이션 끊김의 기여 경로를 조사한다. 정상 낙하·복구와 기존 NPC 파이프라인은 유지한다. 기능·시각 품질·성능 수용을 분리한다.

2. 실제 수행

고정 실행파일의 TickCount 문자열 등록, wrapper, QPC leaf와 PE import를 제한된 Capstone 디코드·해시·바이트 검사로 연결했다. REA MCP나 Ghidra 분석 성공으로 보고하지 않는다. TickCount(0)는 초기화된 정상 QPC 경로에서 QPC와 엔진 원점 qword의 차이의 signed low32다. 다른 인자의 일반 의미로 확대하지 않는다.

기존 단일 helper 진입 관측에 원점·주파수 읽기와 실제 QPC import/export 일치 검사를 추가했다. 실제 실행 JS와 driver를 각 실행에 보관했다. native 인자/반환값·원본 상태 override와 내부/반환 hook은 없다. 프로세스 hook을 사용한 관측이며 순수 외부 파일 읽기만 한 실험은 아니다. 별도 nonce/ACK 수집기는 프로세스 메모리를 읽지 않는다.

동일1개체/8 render part·AI/이동 OFF·고정 달리기·VSync ON·120초3실행과45초 녹화를 정상 종료했다. 첫 실행의 파일 시계 watcher는 Seconds 변수 충돌로 실패했고 뒤늦은 수집도0 ACK/4 timeout이었다. 첫 실행은 독립 시계 확인과 native/CPU/영상 연결에서 제외했다. 첫 실행의 약52ms native 호출 간격은 뒤의 실행과 다르므로 같은 비교 반복에 넣지 않는다. 실패 watcher 원본 snapshot은 없으며 console 기록만 남았다.

뒤2회는 각각10초 독립 파일 교정과 실제 engine origin 읽기를 교차 검사했다. 영상21~24초/24~27초의 HUD ID를 저장된 본 조회와 연결했다. 원점 정수의 정확한 일치는 GPU/Present나 전체 관측 오차0을 뜻하지 않는다. 실제 창 활성화 시점은 전 구간에서 동일하게 확립되지 않았으며 녹화는 가려진 창도 담을 수 있다.

3. 측정 결과

|실행 ID|native 호출/인접쌍|CPU 연결쌍|영상 삼자 연결|HUD dt p50/p95/p99 ms|
|---|---|---:|---:|---|
|runtime-20261028-074826|154/153|0|0|20.094/34.643625/36.39955|
|runtime-20261028-075329|401/400|400|296|20.037/35.22346/37.11252|
|runtime-20261028-075921|399/398|398|294|19.9695/34.515485/36.332647|

뒤2회만 합산하면 native 원점 읽기800개, 연속 native/CPU 연결798쌍, native/CPU/실제 영상 연결590쌍, 정확 영상 HUD720/720프레임이다. 독립 파일 교정 ACK는241/245개, timeout0이며 구간 폭8.1986/7.8583ms 안에 같은 실행의 직접 읽은 원점이 포함됐다. 직접 원점 읽기 bracket 중앙값0.0166/0.0163ms는 읽기 구간이며 전체 observer 비용이 아니다.

25ms 이상 입력만 구분한 기술적 집계에서 reset 표본73/71쌍의 입력 중앙값은 각각34ms였지만 모델 포즈 진행량은4/3ms,6본 source-FK proxy는 모두4.1667ms였다. 비reset75/74쌍에서는 입력34ms, 모델34ms, proxy37.5/33.3333ms였다. 영상 실루엣 변화 중앙값은 reset797/656px, 비reset3797/3835px였다. 픽셀 변화는 자세 곡률·투영·임계값 영향을 받으며 주관적 품질 점수가 아니다.

native 모델과 실제 픽셀 변화 상관0.94879/0.95140, CPU 포즈와 픽셀 변화 상관0.94864/0.95107. native-6본 proxy 잔차 RMS2.7625/3.1053ms. 직접 graph/clip phase 측정은 아니다. 첫 실행을 포함한3회의 float32 누적기 전이 잔차는0이다. 첫 실행은0→0 reset153개여서 단순 감소 횟수0과 구별해야 한다.

8/64 기능 회귀와64 비용 실험은 이번 미실행이다. 표는1개체 관측·녹화 환경의 HUD 간격이며 native animation FPS나 GPU frame time이 아니다. 최대80.6366ms도 필터링하지 않았다.20ms p95 달성이나 성능 개선으로 판정하지 않는다.

4. 원인 분석

확인: 정확 float32 누적기 reset 경계에서 긴 입력에 비해 작은 CPU 포즈 진행량이 나타나며 같은 프레임의 실제 영상 변화도 작다. 기존 시계 원점 가정의 불확실성을 뒤2회에서 직접 원점 읽기·독립 교정으로 해소했다. 이전에 연결 표본이10쌍뿐이던 결과는 그대로 보존하며 소급 수정하지 않는다.

추정: 해당 reset 경로가 보이는 시간적 불균등에 기여한다는 관측 근거가 강해졌다. 아직 반사실적 단일요인 개입이 없으므로 단독 원인·모든14동작의 원인·GPU 표시 단계 확정으로 확대하지 않는다. 이전 평균-rate 보정 후보가 부드러움을 해결하지 못했다는 결과도 유지한다.

분석기 수정:0→0 경계를 감소 횟수만으로 세면 누적기 소비량을0으로 오판했다. 모델 threshold crossing으로 계산하도록 수정했고 빈 cycle 통계는 null로 보존했다. 기존 잘못된 분석은 덮어쓰지 않고 v3와 correction sidecar로 교체 범위를 명시했다.11개 오프라인 회귀는 통과했지만 게임 기능 회귀가 아니다.

5. 검증 수준

진단 성공: 고정 실행파일 clock binding·runtime import/원점 guard와 세 실행의 저장된 관측.
반복 검증 완료: 뒤2회의 native/CPU/실제 영상 시간 연결 및 reset 경계의 작은 몸 진행량 관측.
회귀 통과:11개 오프라인 시계·float32·0→0 ledger 검사.
미실행/미수용: 원인 제거 후보, 모든 애니메이션의 지각적 품질,8/64 새 기능 회귀,64 비용 귀속·할당/GC, GPU/Present. 최종 수용 미완료다.

6. 변경 사항

관측·분석 도구만 수정했다. TickCount binding 검사, guarded origin 단일 entry 관측, watcher 변수 충돌/부족 ACK 거부, 정확 native/CPU/영상 join과0→0 ledger 회귀를 추가했다. 각 실행의 archived source가 실제 실행 범위를 결정하며 이후 추가한 preflight를 과거 실행에 소급하지 않는다.

B8FB integration/453D base/0C6E assets 유지. PBO·그래프·모델·AI 동작 변경과 재서명은 없고 실행마다 기존3 PBO 서명 검증 성공을 유지했다. 운영 서버·기존 보고서·원시 로그/영상은 변경하지 않았다.

7. 원시 증거

실행074826/075329/075921의 mission/launch, 실제 observer JS·driver/config/status/calls, HUD/본/시계 기록, finalize된 영상, 영상 ID·CPU 연결 JSON과 static binding/import 검사를 보존했다. 아래 목록은 저장 전2회 해시·메타데이터 안정성 검사 대상이다. 원본영상·로컬경로·프로세스 정보·원점 정수는 공개하지 않는다. 공개 검토 패킷은 집계와 처음8개 연결행만 발췌하며 전체 trace가 아니다.

8. 잔여 문제

다음은 지원되는 graph/시뮬레이션 시간 제어가 실제로 어디에 적용되는지 확인하고, 필요하면 원본 상태를 바꾸지 않는 격리 단일요인 반사실적 실험을 설계한다. 표시 ratio가 계산되는 위치·인자 수명·원본 비변경을 먼저 확인하며 임의 보간·tick 강제 갱신을 제품에 적용하지 않는다. 후보 비교는 같은 조건의 순서 교차 반복과 영상·CPU 잔차를 함께 평가한다.

64 기능은 마지막 실제64/64 최종 도착과 고지대13/14/43 미도달·EGRESS_STALL1을 별도로 유지한다. climb-end failure0은 해당 과거 실행 범위다.1개체 진단으로 기능 수용을 대체하지 않는다.64 비용은 정지 애니메이션1/64·활성 정적포즈64·활성 애니메이션64의 별도 통제 실험으로 귀속해야 하며20ms p95는 목표다. 미계측 native graph 세부 비용·GPU/Present·할당/GC는 미확보로 남긴다.

원본 보고서 ID: 2026-10-28_081756_702387+0900_npc-tickcount-origin.md
원본 SHA-256: 735d8c4dacf0d7fed011052e75106dff5cabf0771e89d97f894e0ceddff0fbb7
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 집계/대표행 (전체 trace 아님)
[
  {
    "summary": {
      "runId": "runtime-20261028-074826",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.094,
        "p95": 34.643625,
        "p99": 36.39955
      },
      "nativeCalls": 154,
      "nativeAdjacentPairs": 153,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 153,
      "frameJoinAdmitted": false,
      "clockAcks": 0,
      "posePairs": 0,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "frameSync": "on",
        "fileClockSeconds": 10,
        "clockBridgeAdmitted": false,
        "performanceAcceptance": false,
        "repetitions": 1,
        "frameWindow": "t>=15s HUD callback begin intervals; app-active, not GPU/Present; actual foreground timing not uniformly established"
      }
    },
    "nativeModel": {
      "adjacentCalls": 153,
      "thresholdCrossings": 153,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 0,
      "maxResidualMs": 0.0,
      "modelResetToResetWallMs": {
        "p05": 43.0,
        "p50": 52.0,
        "p95": 62.44999999999999,
        "p99": 69.0
      },
      "observedInputSumMs": 7950.000014156103,
      "consumedThresholdsMs": 7650.000113993883,
      "retainedAccumulatorDeltaMs": 0,
      "thresholdOvershootSumMs": 299.99990016222,
      "thresholdOvershootMs": {
        "p05": 0.9999983012676239,
        "p50": 2.0000003278255463,
        "p95": 2.99999862909317,
        "p99": 4.0000006556510925
      },
      "conservationResidualMs": 0.0,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "analysisSha256": "65eb61d8de300d75c4787def764da8464d5a73bc589642178f31ee372f52dd28",
    "limitations": [
      "Observer and file clock exchange can perturb timing; no optimization acceptance.",
      "Only clock-interval-definite events and consecutive unique HUD pairs are joined. Ambiguous observations remain excluded.",
      "FK phase is a six-point proxy, not direct clip phase. No graph completion or GPU/Present timestamp.",
      "No new8/64 functional regression or universal cause/smoothness acceptance."
    ]
  },
  {
    "summary": {
      "runId": "runtime-20261028-075329",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 20.037,
        "p95": 35.22346,
        "p99": 37.11252
      },
      "nativeCalls": 401,
      "nativeAdjacentPairs": 400,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 103,
      "frameJoinAdmitted": true,
      "clockAcks": 241,
      "posePairs": 400,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "frameSync": "on",
        "fileClockSeconds": 10,
        "clockBridgeAdmitted": true,
        "performanceAcceptance": false,
        "repetitions": 1,
        "frameWindow": "t>=15s HUD callback begin intervals; app-active, not GPU/Present; actual foreground timing not uniformly established"
      },
      "videoTriples": 296,
      "exactVideoHudFrames": 360,
      "modelVisibleCorrelation": 0.9487899146723037,
      "cpuVisibleCorrelation": 0.9486398459672786
    },
    "nativeModel": {
      "adjacentCalls": 400,
      "thresholdCrossings": 103,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 103,
      "maxResidualMs": 0.0,
      "observedInputSumMs": 7991.0001661628485,
      "consumedThresholdsMs": 5150.00007674098,
      "retainedAccumulatorDeltaMs": 8.00000037997961,
      "thresholdOvershootSumMs": 2832.999996840954,
      "thresholdOvershootMs": {
        "p05": 3.7000022828578985,
        "p50": 29.999997466802597,
        "p95": 32.00000151991844,
        "p99": 33.980005532503135
      },
      "conservationResidualMs": 9.220093488693237e-05,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "analysisSha256": "bb374257c72863dcd376542fe8970ca43de7089d30be47043fe8cf56c5bb858d",
    "limitations": [
      "Observer and file clock exchange can perturb timing; no optimization acceptance.",
      "Only clock-interval-definite events and consecutive unique HUD pairs are joined. Ambiguous observations remain excluded.",
      "FK phase is a six-point proxy, not direct clip phase. No graph completion or GPU/Present timestamp.",
      "No new8/64 functional regression or universal cause/smoothness acceptance."
    ],
    "visibleChain": {
      "nativeHudPosePairs": 400,
      "exactVideoHudFrames": 360,
      "nativePoseVideoTriples": 296,
      "groups": {
        "reset": {
          "pairs": 77,
          "inputDtMs": {
            "n": 77,
            "p50": 34.00000184774399,
            "p95": 37.00000047683716,
            "p99": 37.00000047683716,
            "maximum": 37.00000047683716
          },
          "modelPoseDeltaMs": {
            "n": 77,
            "p50": 4.0000006556510925,
            "p95": 11.999998986721039,
            "p99": 18.68000075221058,
            "maximum": 24.000000208616257
          },
          "sourceProxyDeltaMs": {
            "n": 77,
            "p50": 4.166666666666667,
            "p95": 12.5,
            "p99": 21.83333333333331,
            "maximum": 25.0
          },
          "visibleMaskChangePixels": {
            "n": 77,
            "p50": 798.0,
            "p95": 1643.0000000000002,
            "p99": 2961.879999999995,
            "maximum": 3712
          }
        },
        "nonreset": {
          "pairs": 219,
          "inputDtMs": {
            "n": 219,
            "p50": 7.000000216066837,
            "p95": 35.00000014901161,
            "p99": 37.00000047683716,
            "maximum": 39.000000804662704
          },
          "modelPoseDeltaMs": {
            "n": 219,
            "p50": 6.999999284744263,
            "p95": 34.99999921768904,
            "p99": 37.000001072883606,
            "maximum": 39.00000173598528
          },
          "sourceProxyDeltaMs": {
            "n": 219,
            "p50": 8.333333333333334,
            "p95": 37.5,
            "p99": 41.666666666666664,
            "maximum": 41.666666666666664
          },
          "visibleMaskChangePixels": {
            "n": 219,
            "p50": 1238.0,
            "p95": 4480.599999999999,
            "p99": 5024.999999999999,
            "maximum": 5128
          }
        },
        "reset_inputAtLeast25ms": {
          "pairs": 73,
          "inputDtMs": {
            "n": 73,
            "p50": 34.00000184774399,
            "p95": 37.00000047683716,
            "p99": 37.00000047683716,
            "maximum": 37.00000047683716
          },
          "modelPoseDeltaMs": {
            "n": 73,
            "p50": 4.0000006556510925,
            "p95": 11.999999731779099,
            "p99": 18.960000723600395,
            "maximum": 24.000000208616257
          },
          "sourceProxyDeltaMs": {
            "n": 73,
            "p50": 4.166666666666667,
            "p95": 12.5,
            "p99": 17.83333333333334,
            "maximum": 20.833333333333332
          },
          "visibleMaskChangePixels": {
            "n": 73,
            "p50": 797.0,
            "p95": 1664.999999999999,
            "p99": 3001.360000000001,
            "maximum": 3712
          }
        },
        "nonreset_inputAtLeast25ms": {
          "pairs": 75,
          "inputDtMs": {
            "n": 75,
            "p50": 34.00000184774399,
            "p95": 37.00000047683716,
            "p99": 38.25999930500985,
            "maximum": 39.000000804662704
          },
          "modelPoseDeltaMs": {
            "n": 75,
            "p50": 34.00000277906656,
            "p95": 36.999999545514584,
            "p99": 38.25999885797501,
            "maximum": 39.00000173598528
          },
          "sourceProxyDeltaMs": {
            "n": 75,
            "p50": 37.5,
            "p95": 38.749999999999986,
            "p99": 41.666666666666664,
            "maximum": 41.666666666666664
          },
          "visibleMaskChangePixels": {
            "n": 75,
            "p50": 3797.0,
            "p95": 4844.599999999999,
            "p99": 5123.56,
            "maximum": 5128
          }
        }
      },
      "nativeModelVsSourceProxyCorrelation": 0.9757207307925541,
      "nativeModelVsVisibleMaskCorrelation": 0.9487899146723037,
      "cpuPoseVsVisibleMaskCorrelation": 0.9486398459672786,
      "limitations": [
        "One fixed-run diagnostic; cannot generalize to all14 animations or active8/64.",
        "Source phase is estimated by six positions on a quarter-key FK grid, not direct clip/graph phase.",
        "Visible changed-pixel count depends on pose curvature, projection and threshold; not a named-joint angular velocity or perceptual quality score.",
        "Clock origin integer agreement does not mean zero GPU/Present uncertainty. Entry timestamps are read brackets after guards.",
        "Observational mechanism chain, without a counterfactual intervention; not sole-cause or optimization acceptance."
      ]
    },
    "representativeRows": [
      {
        "firstFrame": 1175,
        "lastFrame": 1176,
        "reset": false,
        "inputDtMs": 4.999999888241291,
        "modelPoseDeltaMs": 4.999999888241291,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": -0.8333332215746244,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.033333333333335,
        "segment": "21.0",
        "bodyMaskChangedPixels": 782,
        "queriedRmsChangeMeters": 0.010573123151021494
      },
      {
        "firstFrame": 1176,
        "lastFrame": 1177,
        "reset": false,
        "inputDtMs": 35.00000014901161,
        "modelPoseDeltaMs": 34.99999921768904,
        "sourceProxyDeltaMs": 41.666666666666664,
        "residualMs": 6.666667448977627,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.05,
        "segment": "21.0",
        "bodyMaskChangedPixels": 3575,
        "queriedRmsChangeMeters": 0.07113419148810694
      },
      {
        "firstFrame": 1177,
        "lastFrame": 1178,
        "reset": false,
        "inputDtMs": 7.000000216066837,
        "modelPoseDeltaMs": 6.999999284744263,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": -2.833332618077596,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.066666666666666,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1079,
        "queriedRmsChangeMeters": 0.013798021228905723
      },
      {
        "firstFrame": 1178,
        "lastFrame": 1179,
        "reset": true,
        "inputDtMs": 30.99999949336052,
        "modelPoseDeltaMs": 3.0000023543834686,
        "sourceProxyDeltaMs": 0.0,
        "residualMs": -3.0000023543834686,
        "callbackDeltaMs": 50.00000000000071,
        "callbackResetResidualMs": 7.45057349149647e-07,
        "videoSeconds": 21.083333333333332,
        "segment": "21.0",
        "bodyMaskChangedPixels": 557,
        "queriedRmsChangeMeters": 0.005874131893309852
      },
      {
        "firstFrame": 1179,
        "lastFrame": 1180,
        "reset": false,
        "inputDtMs": 7.000000216066837,
        "modelPoseDeltaMs": 7.000000216066837,
        "sourceProxyDeltaMs": 8.333333333333334,
        "residualMs": 1.333333117266496,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.1,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1117,
        "queriedRmsChangeMeters": 0.01476248596386575
      },
      {
        "firstFrame": 1180,
        "lastFrame": 1181,
        "reset": false,
        "inputDtMs": 32.999999821186066,
        "modelPoseDeltaMs": 32.99999888986349,
        "sourceProxyDeltaMs": 33.333333333333336,
        "residualMs": 0.3333344434698418,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.133333333333333,
        "segment": "21.0",
        "bodyMaskChangedPixels": 3270,
        "queriedRmsChangeMeters": 0.07328240388046779
      },
      {
        "firstFrame": 1181,
        "lastFrame": 1182,
        "reset": false,
        "inputDtMs": 8.00000037997961,
        "modelPoseDeltaMs": 8.000001311302185,
        "sourceProxyDeltaMs": 8.333333333333334,
        "residualMs": 0.33333202203114815,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.15,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1097,
        "queriedRmsChangeMeters": 0.018849359697347818
      },
      {
        "firstFrame": 1182,
        "lastFrame": 1183,
        "reset": true,
        "inputDtMs": 32.00000151991844,
        "modelPoseDeltaMs": 2.0000003278255463,
        "sourceProxyDeltaMs": 0.0,
        "residualMs": -2.0000003278255463,
        "callbackDeltaMs": 49.999999999998934,
        "callbackResetResidualMs": 7.450591255064865e-07,
        "videoSeconds": 21.166666666666668,
        "segment": "21.0",
        "bodyMaskChangedPixels": 359,
        "queriedRmsChangeMeters": 0.00477645047777809
      }
    ],
    "rowSelection": "First8 admitted joinedPairs in video-time order; not the complete trace.",
    "independentClockWidthMs": 8.1986,
    "poseProxyResidualRmsMs": 2.7625218085915844
  },
  {
    "summary": {
      "runId": "runtime-20261028-075921",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "scenario": "anim1",
      "count": 1,
      "durationSeconds": 120,
      "highHoldWindow": null,
      "omitGameplayMetrics": true,
      "foregroundMissionFrameMs": {
        "p50": 19.9695,
        "p95": 34.515485,
        "p99": 36.332647
      },
      "nativeCalls": 399,
      "nativeAdjacentPairs": 398,
      "nativeModelMaxResidualMs": 0.0,
      "resetCrossings": 101,
      "frameJoinAdmitted": true,
      "clockAcks": 245,
      "posePairs": 398,
      "measurementConditions": {
        "logicalAgents": 1,
        "renderParts": 8,
        "clip": "run",
        "ai": false,
        "movement": false,
        "nativeEntryObserver": true,
        "nativeStateOverrides": false,
        "frameSync": "on",
        "fileClockSeconds": 10,
        "clockBridgeAdmitted": true,
        "performanceAcceptance": false,
        "repetitions": 1,
        "frameWindow": "t>=15s HUD callback begin intervals; app-active, not GPU/Present; actual foreground timing not uniformly established"
      },
      "videoTriples": 294,
      "exactVideoHudFrames": 360,
      "modelVisibleCorrelation": 0.9514002046648427,
      "cpuVisibleCorrelation": 0.9510665784849778
    },
    "nativeModel": {
      "adjacentCalls": 398,
      "thresholdCrossings": 101,
      "method": "float32 ADDSS followed by exact COMISS/JB threshold; no comparison epsilon",
      "descriptorDecreases": 101,
      "maxResidualMs": 0.0,
      "modelResetToResetWallMs": {
        "p05": 60.6,
        "p50": 80.0,
        "p95": 82.0,
        "p99": 84.47000000000024
      },
      "observedInputSumMs": 7969.0001597628,
      "consumedThresholdsMs": 5050.000075250864,
      "retainedAccumulatorDeltaMs": 46.00000008940697,
      "thresholdOvershootSumMs": 2873.0000406503677,
      "thresholdOvershootMs": {
        "p05": 10.000001639127731,
        "p50": 29.999997466802597,
        "p95": 32.00000151991844,
        "p99": 33.000003546476364
      },
      "conservationResidualMs": 4.377216100692749e-05,
      "schedulerAccumulatorOvershootDiscardObserved": true,
      "graphPhaseLossProven": false,
      "evaluationTimeArgumentCaptured": false
    },
    "analysisSha256": "80a750324883731e7105eb645cc66705ad8f719b6e49031cd6972785ac5aa27d",
    "limitations": [
      "Observer and file clock exchange can perturb timing; no optimization acceptance.",
      "Only clock-interval-definite events and consecutive unique HUD pairs are joined. Ambiguous observations remain excluded.",
      "FK phase is a six-point proxy, not direct clip phase. No graph completion or GPU/Present timestamp.",
      "No new8/64 functional regression or universal cause/smoothness acceptance."
    ],
    "visibleChain": {
      "nativeHudPosePairs": 398,
      "exactVideoHudFrames": 360,
      "nativePoseVideoTriples": 294,
      "groups": {
        "reset": {
          "pairs": 76,
          "inputDtMs": {
            "n": 76,
            "p50": 33.50000083446503,
            "p95": 35.00000014901161,
            "p99": 46.99999839067459,
            "maximum": 79.99999821186066
          },
          "modelPoseDeltaMs": {
            "n": 76,
            "p50": 3.999996930360794,
            "p95": 9.999998845160007,
            "p99": 13.500000350177288,
            "maximum": 15.000000596046448
          },
          "sourceProxyDeltaMs": {
            "n": 76,
            "p50": 4.166666666666667,
            "p95": 12.5,
            "p99": 20.833333333333332,
            "maximum": 20.833333333333332
          },
          "visibleMaskChangePixels": {
            "n": 76,
            "p50": 670.5,
            "p95": 1517.0,
            "p99": 2068.75,
            "maximum": 2683
          }
        },
        "nonreset": {
          "pairs": 218,
          "inputDtMs": {
            "n": 218,
            "p50": 7.000000216066837,
            "p95": 34.00000184774399,
            "p99": 35.00000014901161,
            "maximum": 35.999998450279236
          },
          "modelPoseDeltaMs": {
            "n": 218,
            "p50": 7.000000216066837,
            "p95": 34.00000277906656,
            "p99": 35.00000108033419,
            "maximum": 35.99999938160181
          },
          "sourceProxyDeltaMs": {
            "n": 218,
            "p50": 8.333333333333334,
            "p95": 37.5,
            "p99": 37.5,
            "maximum": 41.666666666666664
          },
          "visibleMaskChangePixels": {
            "n": 218,
            "p50": 1321.0,
            "p95": 4310.5,
            "p99": 4904.3,
            "maximum": 5002
          }
        },
        "reset_inputAtLeast25ms": {
          "pairs": 71,
          "inputDtMs": {
            "n": 71,
            "p50": 34.00000184774399,
            "p95": 35.00000014901161,
            "p99": 49.199998378753534,
            "maximum": 79.99999821186066
          },
          "modelPoseDeltaMs": {
            "n": 71,
            "p50": 3.0000023543834686,
            "p95": 5.499999970197678,
            "p99": 13.60000036656856,
            "maximum": 15.000000596046448
          },
          "sourceProxyDeltaMs": {
            "n": 71,
            "p50": 4.166666666666667,
            "p95": 12.5,
            "p99": 20.833333333333332,
            "maximum": 20.833333333333332
          },
          "visibleMaskChangePixels": {
            "n": 71,
            "p50": 656.0,
            "p95": 1073.0,
            "p99": 2109.6999999999975,
            "maximum": 2683
          }
        },
        "nonreset_inputAtLeast25ms": {
          "pairs": 74,
          "inputDtMs": {
            "n": 74,
            "p50": 34.00000184774399,
            "p95": 35.00000014901161,
            "p99": 35.999998450279236,
            "maximum": 35.999998450279236
          },
          "modelPoseDeltaMs": {
            "n": 74,
            "p50": 34.00000277906656,
            "p95": 34.99999986961484,
            "p99": 35.99999938160181,
            "maximum": 35.99999938160181
          },
          "sourceProxyDeltaMs": {
            "n": 74,
            "p50": 33.333333333333336,
            "p95": 37.5,
            "p99": 41.666666666666664,
            "maximum": 41.666666666666664
          },
          "visibleMaskChangePixels": {
            "n": 74,
            "p50": 3835.0,
            "p95": 4849.2,
            "p99": 5000.54,
            "maximum": 5002
          }
        }
      },
      "nativeModelVsSourceProxyCorrelation": 0.9726900778199286,
      "nativeModelVsVisibleMaskCorrelation": 0.9514002046648427,
      "cpuPoseVsVisibleMaskCorrelation": 0.9510665784849778,
      "limitations": [
        "One fixed-run diagnostic; cannot generalize to all14 animations or active8/64.",
        "Source phase is estimated by six positions on a quarter-key FK grid, not direct clip/graph phase.",
        "Visible changed-pixel count depends on pose curvature, projection and threshold; not a named-joint angular velocity or perceptual quality score.",
        "Clock origin integer agreement does not mean zero GPU/Present uncertainty. Entry timestamps are read brackets after guards.",
        "Observational mechanism chain, without a counterfactual intervention; not sole-cause or optimization acceptance."
      ]
    },
    "representativeRows": [
      {
        "firstFrame": 1178,
        "lastFrame": 1179,
        "reset": true,
        "inputDtMs": 34.00000184774399,
        "modelPoseDeltaMs": 4.0000006556510925,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": 0.16666601101557407,
        "callbackDeltaMs": 50.00000000000071,
        "callbackResetResidualMs": 7.45057349149647e-07,
        "videoSeconds": 21.016666666666666,
        "segment": "21.0",
        "bodyMaskChangedPixels": 656,
        "queriedRmsChangeMeters": 0.012873476460019146
      },
      {
        "firstFrame": 1179,
        "lastFrame": 1180,
        "reset": false,
        "inputDtMs": 6.000000052154064,
        "modelPoseDeltaMs": 6.000000052154064,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": -1.8333333854873977,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.033333333333335,
        "segment": "21.0",
        "bodyMaskChangedPixels": 925,
        "queriedRmsChangeMeters": 0.017109637123562834
      },
      {
        "firstFrame": 1180,
        "lastFrame": 1181,
        "reset": false,
        "inputDtMs": 34.00000184774399,
        "modelPoseDeltaMs": 34.00000277906656,
        "sourceProxyDeltaMs": 37.5,
        "residualMs": 3.499997220933436,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.05,
        "segment": "21.0",
        "bodyMaskChangedPixels": 3465,
        "queriedRmsChangeMeters": 0.09780925776735042
      },
      {
        "firstFrame": 1181,
        "lastFrame": 1182,
        "reset": false,
        "inputDtMs": 7.000000216066837,
        "modelPoseDeltaMs": 6.999999284744263,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": -2.833332618077596,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.083333333333332,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1322,
        "queriedRmsChangeMeters": 0.020701840260389088
      },
      {
        "firstFrame": 1182,
        "lastFrame": 1183,
        "reset": true,
        "inputDtMs": 32.00000151991844,
        "modelPoseDeltaMs": 2.99999862909317,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": 1.1666680375734964,
        "callbackDeltaMs": 50.00000000000071,
        "callbackResetResidualMs": 7.45057349149647e-07,
        "videoSeconds": 21.1,
        "segment": "21.0",
        "bodyMaskChangedPixels": 740,
        "queriedRmsChangeMeters": 0.008926984270551101
      },
      {
        "firstFrame": 1183,
        "lastFrame": 1184,
        "reset": false,
        "inputDtMs": 7.000000216066837,
        "modelPoseDeltaMs": 7.000000216066837,
        "sourceProxyDeltaMs": 8.333333333333334,
        "residualMs": 1.333333117266496,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.116666666666667,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1290,
        "queriedRmsChangeMeters": 0.01990307817901541
      },
      {
        "firstFrame": 1184,
        "lastFrame": 1185,
        "reset": false,
        "inputDtMs": 34.00000184774399,
        "modelPoseDeltaMs": 34.00000091642141,
        "sourceProxyDeltaMs": 33.333333333333336,
        "residualMs": -0.6666675830880806,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.133333333333333,
        "segment": "21.0",
        "bodyMaskChangedPixels": 3624,
        "queriedRmsChangeMeters": 0.1028724070090388
      },
      {
        "firstFrame": 1185,
        "lastFrame": 1186,
        "reset": false,
        "inputDtMs": 6.000000052154064,
        "modelPoseDeltaMs": 6.000000983476639,
        "sourceProxyDeltaMs": 4.166666666666667,
        "residualMs": -1.8333343168099723,
        "callbackDeltaMs": 0.0,
        "callbackResetResidualMs": 0.0,
        "videoSeconds": 21.15,
        "segment": "21.0",
        "bodyMaskChangedPixels": 1172,
        "queriedRmsChangeMeters": 0.019279431090326996
      }
    ],
    "rowSelection": "First8 admitted joinedPairs in video-time order; not the complete trace.",
    "independentClockWidthMs": 7.858300000000001,
    "poseProxyResidualRmsMs": 3.1052738133655358
  }
]

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
