# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261028T192749-period-visible-20261029

표시 비율 후보 거부 — 같은6초 영상에서 세대경계 점프 확인

작성: 2026-10-29T04:27:19.349624+09:00

1. 작업 목표

이전 단일 대조/후보의 영상 구간 길이와 native 적용 구간의 차이를 제거하고, 표시 비율 후보가 실제 화면·CPU 포즈의 시간 변화에 미치는 영향을 확인한다. 지각 품질·평균 재생속도·64성능을 분리하며 기존 AI/정상 낙하·복구는 보존한다.

2. 실제 수행

새 게임 실행은 하지 않았다. 정상 종료/저장된 runtime-20261029-040750 대조와041212 후보를 다시 분석했다. 정확HUD의 mission-video 시간차와73본 renderer의 ready 구간을 기준으로0.5초 이후부터6초를 선정했다. 대조 video17.152/20.152초, 후보21.032/24.032초에서3초씩 추출했다. 원본 분석은 유지하고 visible-period-window-*로 별도 저장했다. HUD360개씩을 모두 정확frame/T/A로 확인하고, 연속프레임과 실제 native ready 구간의 교집합만 비교했다.

프레임 간격에 따른 변화도 보려고 CPU6본 RMS/화면 mask 픽셀 변화량을 native 진입간격으로 나눴다. GPU/실제 표시 지속시간으로 오해하지 않는다. 큰 속도 표본을 원래 native graph 세대 경계·이전/다음 비율·이전/이번 관측 주기에 연결했다. 후보 적용 가드4검사는 실제 함수로 실행했고 통과했다. 같은 해시의 거부 모델은 재적용 전에 차단하며, 변경된 모델을 자동 승인하는 것은 아니다.

3. 측정 결과

|관측|대조|후보|
|---|---:|---:|
|정확 HUD 표본|360|360|
|연속 실제 프레임 쌍|298|296|
|동일HUD 반복|60|61|
|동일HUD 중 몸체 변화≥10px|0|0|
|CPU 변화 p95 m|0.118919|0.079129|
|CPU 변화/진입간격 p99 m/s|5.417936|7.423186|
|화면 mask 변화 p95 px|4264.850000|3401.500000|
|CPU/화면 변화 상관|0.944018|0.915159|

두 구간은6초씩이며 native ready 밖 표본0이다. 후보에서 연속HUD가 아닌1쌍은 제외했다. CPU 변화 p95와 화면 변화 p95는 작아졌지만 CPU 변화/진입간격 p99는5.418→7.423m/s로 커졌다. 이것은6본 위치 변화의 파생치이지 개체 이동속도가 아니다. 같은HUD 반복에서 몸체 mask 변화≥10px는 양쪽0이었다.

후보 연속296쌍 중 세대 경계77쌍·내부219쌍. 탐색적 기준(전달 phase 증가/(동일실행 평균graph진행률×진입간격)>2.5)을 넘은5쌍 전부 세대 경계였고 CPU 변화율 상위5개도 모두 그 경계였다. 내부 비율p95는1.157, 경계p95는4.176이었다. 대조에서는 기준 초과1쌍이 내부에 있었다. 표본들이 서로 독립인 반복실험은 아니다.

대표 후보 F1459→1460: 진입간격8.9894ms, 이전 비율0.502388→새 비율0, 표시 phase 증가24.879813ms, 화면 변화3558px. 이전 관측 주기80.5331ms에서 새 주기49.1132ms로 짧아졌다. 나머지4개도7.9411–9.5562ms 간격에23.274–25.316ms 진행했다. 전달 phase 증가 항등식의 수치 잔차는 대표5개 모두0ms다.

이번은 오프라인 재분석이므로 새 미션 p50/p95/p99,8/64 안정성,AI/경로/접촉 비용이나 할당/GC 수치를 생성하지 않는다. 앞선 실행의 전체 미션 p50/p95/p99는 대조20.140/35.267/37.061ms, 후보21.152/34.678/36.374ms이며 새 측정이 아니다.20ms p95 성능 수용은 여전히 미완료다.

4. 원인 분석

관찰된 사실: previous-period 예측 후보는 세대 내부의 전달 시간 증가를 평균graph진행률에 가깝게 만들었지만, 세대가 예상보다 일찍 바뀌면 이전 endpoint까지 남은 변화가 짧은 경계 간격에 몰렸다. 대표5건의 실제 입력·CPU 포즈·화면 변화가 함께 연결된다. 이 후보는 거부한다. 낮은 평균 잔차 하나로 성공을 선언하지 않는다.

추정/한계: 자연 동작의 자세·투영·가림도 mask 변화량에 영향을 준다. source pose 점유를 일치시킨 실험이나 명명된 관절 광학추적이 아니므로 픽셀 p95 감소를 부드러움 점수로 바꾸지 않는다. 거부에 사용한2.5 기준은 이pilot를 본 뒤 정한 탐색 기준이다. 다음 전향적 시험에서는 사전 정의해야 한다. 평균graph속도 약0.64 원인은 이번 분석으로 해결되지 않았다.

반증: 후보의 전체 평균 시간 잔차가 줄었으므로 모든 구간이 더 부드러워졌다는 가정은 경계 큰 점프와 맞지 않는다. 반대로 전체 렌더러가 CPU와 무관하게 몸체를 정지시키는 현상을 이6초 구간에서 확인한 것도 아니다. CPU/화면 상관과 동일HUD 반복 결과는 제한된 관측 근거이며 GPU scheduling 전체를 증명하지 않는다.

5. 검증 수준

진단 성공: 적용 구간이 같은 길이의 실제 영상/CPU 비교와 후보 세대경계 점프의 수치 연결. 회귀 통과: 거부 가드4개 도구 검사만 해당한다. 반복 검증 완료·시각 품질 최종 수용·배포 수정·8→64 기능 회귀·성능 수용은 미완료다. 후보는 원상 적용 유지가 아니라 비채택이다.

6. 변경 사항

영상/정확HUD/포즈연결 분석기에 출력 파일명 옵션과 기존 파일 덮어쓰기 거부를 추가했다. 전용 window/boundary 분석기와 candidate-review.json을 추가하고 trace_horde_native_calls.py가 같은 모델 해시의 거부 후보를 적용 전에 차단하게 했다. 기본 관측plan과 기존PBO/AI는 유지한다. B8FB/453D/0C6E PBO는 변경/재빌드/재서명하지 않았고 앞선 두 실행의3개 서명 검증 성공 상태를 인용한다. 이번에는 새 실행·새 서명 검사를 하지 않았다.

공개 스냅샷 도구는 새 실험 없이 기존 실행을 재분석한 경우 기존 sourceRunId를 참조하도록 보완했다. 과거 결과나 원시 로그를 덮어쓰지 않고 새 체크포인트/새 공개실험 항목으로 보존한다.

7. 원시 증거

두 기존 실행의 launch/녹화 종료, native3200행씩·HUD·분석 해시와 새6초 영상 파생3종씩, window-comparison.json, boundary-outliers.json, candidate-review.json을 연결했다. 영상은 재인코딩하지 않고 같은 확정 영상 해시의 입력에서 추출했다. 공개 패킷에는 집계·상위5개 경계 표본·자체 분석 함수 발췌를 싣는다. 전체 내부 trace/영상/설치자산/프로세스 정보는 공개하지 않는다. 기존 게시기로 저장 전/후 무결성 검사한다.

8. 잔여 문제

같은previous-period 후보를 수정안처럼 반복하지 않는다. 다음은 평균graph시간 약0.64가 발생하는 실제dt 전달/소비 지점을 읽기 중심으로 추적하고, 현재평가 주기 변화와 가변 프레임간격의 관계를 확인하는 것이다. 안정된 원인을 확보하기 전에 새 보간/지연/주기 정책을 배포하지 않는다. 새로운 후보를 만들더라도 세대 내부뿐 아니라 경계 큰 점프와 원본 source 적합오차를 함께 수용 기준에 포함해야 한다.

64 기능은 기존 최종64/64·HIGH_HOLD 미도달13/14/43·EGRESS_STALL1·climb-end0을 유지하며 새회귀는 별도다.64 비용 귀속·20ms p95·사용자 SetTemperature 예외도 별도 미해결이다. 이번재분석으로 기능/시각/성능 목표를 축소하거나 완료로 바꾸지 않는다.

원본 보고서 ID: 2026-10-29_042719_349624+0900_npc-period-visible-boundary.md
원본 SHA-256: 67447eaa0ba974eb563cfa60c7d816ab7b8d2be94232bc6ea0a33c2b57c8cb0e
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 집계와 상위5개 CPU변화율 경계 표본 — 전체 trace 아님
{
  "runs": [
    {
      "runId": "runtime-20261029-040750",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "numericOverride": false,
      "videoSeconds": 6.000000000000002,
      "exactHudSamples": 360,
      "consecutivePairs": 298,
      "excluded": {},
      "duplicateHudPairs": 60,
      "duplicateHudBodyChangesAtLeast10": 0,
      "nativeIntervalMs": {
        "n": 298,
        "p50": 24.81455,
        "p95": 35.53269499999999,
        "p99": 37.34161899999999,
        "maximum": 38.9437
      },
      "cpuMovementMeters": {
        "n": 298,
        "p50": 0.02019422689337015,
        "p95": 0.11891939471261913,
        "p99": 0.13347781184579283,
        "maximum": 0.14611644960954942
      },
      "visibleChangePixels": {
        "n": 298,
        "p50": 1164.5,
        "p95": 4264.849999999999,
        "p99": 4909.36,
        "maximum": 5230
      },
      "cpuMovementMetersPerSecond": {
        "n": 298,
        "p50": 2.965695511608761,
        "p95": 4.630622399505879,
        "p99": 5.417935841242184,
        "maximum": 6.022895801070408
      },
      "visibleChangePixelsPerSecond": {
        "n": 298,
        "p50": 131295.01871066605,
        "p95": 267965.2669835505,
        "p99": 310104.09199691564,
        "maximum": 372510.09346211323
      },
      "cpuVsVisibleCorrelation": 0.944017893276576,
      "cpuSpeedVsVisibleRateCorrelation": 0.8488732156629769,
      "intervalVsVisibleChangeCorrelation": 0.40873902162518205,
      "intervalVsCpuMovementCorrelation": 0.4544316770861031
    },
    {
      "runId": "runtime-20261029-041212",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "numericOverride": true,
      "videoSeconds": 6.000000000000002,
      "exactHudSamples": 360,
      "consecutivePairs": 296,
      "excluded": {
        "nonconsecutiveHUD": 1
      },
      "duplicateHudPairs": 61,
      "duplicateHudBodyChangesAtLeast10": 0,
      "nativeIntervalMs": {
        "n": 296,
        "p50": 28.4462,
        "p95": 34.953025000000004,
        "p99": 36.566885,
        "maximum": 37.21060000000001
      },
      "cpuMovementMeters": {
        "n": 296,
        "p50": 0.03468998566575782,
        "p95": 0.07912919402378113,
        "p99": 0.09891069009094494,
        "maximum": 0.13387072661589114
      },
      "visibleChangePixels": {
        "n": 296,
        "p50": 1629.0,
        "p95": 3401.5,
        "p99": 3827.4500000000025,
        "maximum": 4848
      },
      "cpuMovementMetersPerSecond": {
        "n": 296,
        "p50": 1.895548318619255,
        "p95": 2.842559267357699,
        "p99": 7.42318562797428,
        "maximum": 10.282814659178865
      },
      "visibleChangePixelsPerSecond": {
        "n": 296,
        "p50": 95144.20296819034,
        "p95": 158471.59738768532,
        "p99": 301727.1205533613,
        "maximum": 471471.20675976883
      },
      "cpuVsVisibleCorrelation": 0.9151588292423861,
      "cpuSpeedVsVisibleRateCorrelation": 0.7625236739515685,
      "intervalVsVisibleChangeCorrelation": 0.8713197053475126,
      "intervalVsCpuMovementCorrelation": 0.8557333072448607
    }
  ],
  "boundary": {
    "runs": [
      {
        "runId": "runtime-20261029-040750",
        "numericOverride": false,
        "selectedPairs": 298,
        "generationBoundaries": 77,
        "interiorPairs": 221,
        "boundaryRateVsMean": {
          "n": 77,
          "p50": 0.1688205801466316,
          "p95": 0.5764383876361018,
          "p99": 1.7976690075063322,
          "maximum": 2.0734202969758715
        },
        "interiorRateVsMean": {
          "n": 221,
          "p50": 1.7670942723102332,
          "p95": 2.222402377105862,
          "p99": 2.2906101875805183,
          "maximum": 2.6966863483376895
        },
        "boundaryCpuSpeed": {
          "n": 77,
          "p50": 0.2880545099876175,
          "p95": 0.8516258722318636,
          "p99": 2.9261023422582877,
          "maximum": 4.211581826629469
        },
        "interiorCpuSpeed": {
          "n": 221,
          "p50": 3.397910995295295,
          "p95": 4.759773144227993,
          "p99": 5.527152144595047,
          "maximum": 6.022895801070408
        },
        "rateSpikesAbove2p5Mean": 1,
        "rateSpikesAtGenerationBoundary": 0,
        "topFiveCpuSpeedAllAtBoundary": false,
        "topFiveCpuSpeed": [
          {
            "firstFrame": 1339,
            "lastFrame": 1340,
            "videoSeconds": 19.602,
            "dtMs": 4.4831,
            "cpuMeters": 0.02700124416577875,
            "visiblePixels": 1670,
            "cpuMetersPerSecond": 6.022895801070408,
            "visiblePixelsPerSecond": 372510.09346211323,
            "submittedDeltaMs": 7.000000216066837,
            "generationBoundary": false,
            "previousFraction": 0.0,
            "newFraction": 0.14000000223517414,
            "previousPeriodMs": 80.2036,
            "newPeriodMs": 80.2036,
            "phaseRateVsMeanRatio": 2.4400769513485026,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1341,
            "lastFrame": 1342,
            "videoSeconds": 19.635333333333335,
            "dtMs": 5.5556,
            "cpuMeters": 0.03110536520173544,
            "visiblePixels": 952,
            "cpuMetersPerSecond": 5.59892094494482,
            "visiblePixelsPerSecond": 171358.62913096696,
            "submittedDeltaMs": 6.999999284744263,
            "generationBoundary": false,
            "previousFraction": 0.7999999701976781,
            "newFraction": 0.939999953806401,
            "previousPeriodMs": 80.2036,
            "newPeriodMs": 80.2036,
            "phaseRateVsMeanRatio": 1.9690236023444683,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1281,
            "lastFrame": 1282,
            "videoSeconds": 18.435333333333336,
            "dtMs": 5.2155,
            "cpuMeters": 0.028974814641339827,
            "visiblePixels": 1231,
            "cpuMetersPerSecond": 5.555520015595787,
            "visiblePixelsPerSecond": 236027.2265362861,
            "submittedDeltaMs": 8.999999612569809,
            "generationBoundary": false,
            "previousFraction": 0.7600000387430186,
            "newFraction": 0.9400000283122059,
            "previousPeriodMs": 82.0951,
            "newPeriodMs": 82.0951,
            "phaseRateVsMeanRatio": 2.6966863483376895,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1507,
            "lastFrame": 1508,
            "videoSeconds": 22.968666666666667,
            "dtMs": 5.053,
            "cpuMeters": 0.027355328377971767,
            "visiblePixels": 719,
            "cpuMetersPerSecond": 5.413680660592077,
            "visiblePixelsPerSecond": 142291.70789629922,
            "submittedDeltaMs": 6.000000983476639,
            "generationBoundary": false,
            "previousFraction": 0.800000044703483,
            "newFraction": 0.9200000625848761,
            "previousPeriodMs": 79.7264,
            "newPeriodMs": 79.7264,
            "phaseRateVsMeanRatio": 1.8556066443124628,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1231,
            "lastFrame": 1232,
            "videoSeconds": 17.435333333333336,
            "dtMs": 7.9544999999999995,
            "cpuMeters": 0.04259993563179801,
            "visiblePixels": 2282,
            "cpuMetersPerSecond": 5.355451082003647,
            "visiblePixelsPerSecond": 286881.6393236533,
            "submittedDeltaMs": 9.999999776482582,
            "generationBoundary": false,
            "previousFraction": 0.0,
            "newFraction": 0.19999999254941953,
            "previousPeriodMs": 80.2374,
            "newPeriodMs": 80.2374,
            "phaseRateVsMeanRatio": 1.9645857957181931,
            "boundaryAdvanceIdentityErrorMs": 0.0
          }
        ],
        "representativeRateSpikes": [
          {
            "firstFrame": 1281,
            "lastFrame": 1282,
            "videoSeconds": 18.435333333333336,
            "dtMs": 5.2155,
            "cpuMeters": 0.028974814641339827,
            "visiblePixels": 1231,
            "cpuMetersPerSecond": 5.555520015595787,
            "visiblePixelsPerSecond": 236027.2265362861,
            "submittedDeltaMs": 8.999999612569809,
            "generationBoundary": false,
            "previousFraction": 0.7600000387430186,
            "newFraction": 0.9400000283122059,
            "previousPeriodMs": 82.0951,
            "newPeriodMs": 82.0951,
            "phaseRateVsMeanRatio": 2.6966863483376895,
            "boundaryAdvanceIdentityErrorMs": 0.0
          }
        ]
      },
      {
        "runId": "runtime-20261029-041212",
        "numericOverride": true,
        "selectedPairs": 296,
        "generationBoundaries": 77,
        "interiorPairs": 219,
        "boundaryRateVsMean": {
          "n": 77,
          "p50": 0.970790313937387,
          "p95": 4.176184118236071,
          "p99": 4.573312684099576,
          "maximum": 4.774810681964716
        },
        "interiorRateVsMean": {
          "n": 219,
          "p50": 0.9870973750089391,
          "p95": 1.1574728823696874,
          "p99": 1.5958348494205692,
          "maximum": 1.611520980806063
        },
        "boundaryCpuSpeed": {
          "n": 77,
          "p50": 1.8156914535166297,
          "p95": 6.898836466032519,
          "p99": 9.594438615228873,
          "maximum": 10.282814659178865
        },
        "interiorCpuSpeed": {
          "n": 219,
          "p50": 1.905298790803315,
          "p95": 2.739657490981957,
          "p99": 3.7239583846910236,
          "maximum": 4.265599660203389
        },
        "rateSpikesAbove2p5Mean": 5,
        "rateSpikesAtGenerationBoundary": 5,
        "topFiveCpuSpeedAllAtBoundary": true,
        "topFiveCpuSpeed": [
          {
            "firstFrame": 1459,
            "lastFrame": 1460,
            "videoSeconds": 26.398666666666667,
            "dtMs": 8.9894,
            "cpuMeters": 0.09243633409722248,
            "visiblePixels": 3558,
            "cpuMetersPerSecond": 10.282814659178865,
            "visiblePixelsPerSecond": 395799.49718557415,
            "submittedDeltaMs": 24.879813194274902,
            "generationBoundary": true,
            "previousFraction": 0.5023884698392679,
            "newFraction": 0.0,
            "previousPeriodMs": 80.53309999999999,
            "newPeriodMs": 49.113200000000006,
            "phaseRateVsMeanRatio": 4.324270851227055,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1209,
            "lastFrame": 1210,
            "videoSeconds": 21.398666666666667,
            "dtMs": 7.9411,
            "cpuMeters": 0.07446414501288522,
            "visiblePixels": 3744,
            "cpuMetersPerSecond": 9.377056706613091,
            "visiblePixelsPerSecond": 471471.20675976883,
            "submittedDeltaMs": 24.26835522055626,
            "generationBoundary": true,
            "previousFraction": 0.5146176291314121,
            "newFraction": 0.0,
            "previousPeriodMs": 79.6298,
            "newPeriodMs": 48.7492,
            "phaseRateVsMeanRatio": 4.774810681964716,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1433,
            "lastFrame": 1434,
            "videoSeconds": 25.882,
            "dtMs": 9.5562,
            "cpuMeters": 0.07489810196081963,
            "visiblePixels": 3017,
            "cpuMetersPerSecond": 7.837644875663927,
            "visiblePixelsPerSecond": 315711.2659843871,
            "submittedDeltaMs": 25.316357612609863,
            "generationBoundary": true,
            "previousFraction": 0.49365758160266904,
            "newFraction": 0.0,
            "previousPeriodMs": 81.6564,
            "newPeriodMs": 49.682700000000004,
            "phaseRateVsMeanRatio": 4.139162434988324,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1477,
            "lastFrame": 1478,
            "videoSeconds": 26.748666666666665,
            "dtMs": 8.202900000000001,
            "cpuMeters": 0.060712714242295795,
            "visiblePixels": 2469,
            "cpuMetersPerSecond": 7.40137198335903,
            "visiblePixelsPerSecond": 300991.1128990966,
            "submittedDeltaMs": 23.27398955821991,
            "generationBoundary": true,
            "previousFraction": 0.534504942081795,
            "newFraction": 0.0,
            "previousPeriodMs": 79.61609999999999,
            "newPeriodMs": 50.5732,
            "phaseRateVsMeanRatio": 4.433022101713031,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1223,
            "lastFrame": 1224,
            "videoSeconds": 21.682,
            "dtMs": 8.1269,
            "cpuMeters": 0.05504514010185946,
            "visiblePixels": 2326,
            "cpuMetersPerSecond": 6.77320258670089,
            "visiblePixelsPerSecond": 286209.99397064076,
            "submittedDeltaMs": 23.457100614905357,
            "generationBoundary": true,
            "previousFraction": 0.5308427210026574,
            "newFraction": 0.0,
            "previousPeriodMs": 79.8276,
            "newPeriodMs": 50.327799999999996,
            "phaseRateVsMeanRatio": 4.509681737405322,
            "boundaryAdvanceIdentityErrorMs": 0.0
          }
        ],
        "representativeRateSpikes": [
          {
            "firstFrame": 1209,
            "lastFrame": 1210,
            "videoSeconds": 21.398666666666667,
            "dtMs": 7.9411,
            "cpuMeters": 0.07446414501288522,
            "visiblePixels": 3744,
            "cpuMetersPerSecond": 9.377056706613091,
            "visiblePixelsPerSecond": 471471.20675976883,
            "submittedDeltaMs": 24.26835522055626,
            "generationBoundary": true,
            "previousFraction": 0.5146176291314121,
            "newFraction": 0.0,
            "previousPeriodMs": 79.6298,
            "newPeriodMs": 48.7492,
            "phaseRateVsMeanRatio": 4.774810681964716,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1223,
            "lastFrame": 1224,
            "videoSeconds": 21.682,
            "dtMs": 8.1269,
            "cpuMeters": 0.05504514010185946,
            "visiblePixels": 2326,
            "cpuMetersPerSecond": 6.77320258670089,
            "visiblePixelsPerSecond": 286209.99397064076,
            "submittedDeltaMs": 23.457100614905357,
            "generationBoundary": true,
            "previousFraction": 0.5308427210026574,
            "newFraction": 0.0,
            "previousPeriodMs": 79.8276,
            "newPeriodMs": 50.327799999999996,
            "phaseRateVsMeanRatio": 4.509681737405322,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1433,
            "lastFrame": 1434,
            "videoSeconds": 25.882,
            "dtMs": 9.5562,
            "cpuMeters": 0.07489810196081963,
            "visiblePixels": 3017,
            "cpuMetersPerSecond": 7.837644875663927,
            "visiblePixelsPerSecond": 315711.2659843871,
            "submittedDeltaMs": 25.316357612609863,
            "generationBoundary": true,
            "previousFraction": 0.49365758160266904,
            "newFraction": 0.0,
            "previousPeriodMs": 81.6564,
            "newPeriodMs": 49.682700000000004,
            "phaseRateVsMeanRatio": 4.139162434988324,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1459,
            "lastFrame": 1460,
            "videoSeconds": 26.398666666666667,
            "dtMs": 8.9894,
            "cpuMeters": 0.09243633409722248,
            "visiblePixels": 3558,
            "cpuMetersPerSecond": 10.282814659178865,
            "visiblePixelsPerSecond": 395799.49718557415,
            "submittedDeltaMs": 24.879813194274902,
            "generationBoundary": true,
            "previousFraction": 0.5023884698392679,
            "newFraction": 0.0,
            "previousPeriodMs": 80.53309999999999,
            "newPeriodMs": 49.113200000000006,
            "phaseRateVsMeanRatio": 4.324270851227055,
            "boundaryAdvanceIdentityErrorMs": 0.0
          },
          {
            "firstFrame": 1477,
            "lastFrame": 1478,
            "videoSeconds": 26.748666666666665,
            "dtMs": 8.202900000000001,
            "cpuMeters": 0.060712714242295795,
            "visiblePixels": 2469,
            "cpuMetersPerSecond": 7.40137198335903,
            "visiblePixelsPerSecond": 300991.1128990966,
            "submittedDeltaMs": 23.27398955821991,
            "generationBoundary": true,
            "previousFraction": 0.534504942081795,
            "newFraction": 0.0,
            "previousPeriodMs": 79.61609999999999,
            "newPeriodMs": 50.5732,
            "phaseRateVsMeanRatio": 4.433022101713031,
            "boundaryAdvanceIdentityErrorMs": 0.0
          }
        ]
      }
    ],
    "threshold": "submitted phase advance / (same-run mean graph rate * entry interval) >2.5; exploratory, not perceptual threshold",
    "candidateAccepted": false,
    "diagnosis": "Test whether previous-period prediction leaves endpoint debt on a short generation-boundary interval.",
    "limitations": [
      "Retrospective attribution, not a new timing policy or exact display timestamp.",
      "The spike threshold was chosen after inspecting this pilot and requires prospective validation.",
      "Model/source pose fit and unmeasured GPU scheduling limit perceptual claims."
    ],
    "inputHashes": {
      "window-comparison.json": "765a52b9922c2abd239757481d93812860cb362dcac36561f162b6cea5684ebd"
    }
  },
  "review": {
    "decision": "rejected",
    "scope": "Previous-period shared display-fraction model; existing B8FB PBO unchanged",
    "modelSha256": "1f64711e3763b6eca6d1bd20c97786d2cb2a928dfbb6396a5e099b8d5fe6f8bf",
    "sourceRuns": [
      "runtime-20261029-040750",
      "runtime-20261029-041212"
    ],
    "reason": "All5 exploratory high phase-rate spikes and top5 CPU-speed outliers occur at generation boundaries; previous fraction about0.5 advances to next buffer on7.94..9.56ms entry intervals.",
    "boundaryEvidenceSha256": "7083bfc10b4876dcd5e0609e90edcc8aa7acccb80033b7c2845c49d1b6bfc9dc",
    "repeatedValidation": false,
    "visualAccepted": false,
    "performanceAccepted": false,
    "next": "Do not repeat unchanged model as a fix. Trace slow graph input/consumption and retain original runtime policy."
  }
}

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
