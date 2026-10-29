# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T083407-fixed-stimulus-cost-20261029

64명 비용 분리 — FOLLOW·접촉 집중,착지 전 정체17번 확인

작성: 2026-10-29T17:32:58.249618+09:00

1. 작업 목표

직전64명 p95 181ms·고지대 미도달28명의 상황에서 누락됐던 전체 이동·상태별·접촉 소비·frame-fall 비용을 측정한다. 본 조회가 큰 병목이라는 추정을 먼저 검증한다. 플레이어 실이동 거리 차이를 줄인 별도 비용 실험을 만들고 기존300초 기능 기준은 보존한다. 새 보간·배속·최적화는 적용하지 않는다.

2. 실제 수행

기존 F6DE 빌드의 opt-in DriveAttribution과 RuntimeTimingProbe를 사용했다. 새 미션은 기존 자연 입력 클래스만 가역 변환했다. 플레이어의 지붕 배치는 유지하고135초에 지면4563/10243,160초에4563/10283으로 배치하며 이동 입력은0이다. NPC 위치·지식·상태·경로·성공 카운터는 쓰지 않았다. 고정 카메라와 동일 상세 계측으로64명·180초 OFF→ON 순서,각1회 실행했다. 두 실행의 설정·미션·PBO·그래픽·control cfg·clip manifest가 같고 profile flag 차이는 post-touchdown-owner.flag 하나임을 검사했다.

기존8/64명300초 기록의 누적 타이머도 같은 프로세스 TickCount와 실제 foreground 시간으로 보정해 재분석했다. 공통 타이머는 actor0에서1회,개체별 합계는 각 개체에서1회만 집계했다. clock의 unsigned32 wrap·중복 개체 공통값·역순/배경 거부에 대한 기존3개 검사가 통과했다. 새 callback 분석은512개 part의 per-body 합계를 다시 합산하며 Mission 타이머는 한 번만 센다. 중첩 elapsed를 CPU self-time으로 바꾸거나 합산하지 않았다.

3. 측정 결과

두 실행에서 지정 지면 세 구간의 XZ 오차는 모두0m,이동 입력0이었다. 실제 player 자극은 일치했지만 NPC 경로·상태 분포·호출량은 달랐다. 이 차이를 숨기지 않는다.

|180초 비용 진단|OFF|ON|
|---|---:|---:|
|미션 p50/p95/p99 ms|97/219/267|106/241/300|
|180초 표본 도착|19/64|13/64|
|고지대 미도달|33|38|
|낙하 후 복귀 에피소드|40|47|
|기록된 등반 종료 실패|0|0|
|CPU callback clock / 실제 시간|0.429157|0.389960|

p95 관측 차이는 ON-OFF=22ms(10.05%)다. 순차1회·다른 NPC 궤적·외부 사용자 앱·상세 계측 조건이므로 인과적인 회귀/개선율이나 재현된 최적화로 판정하지 않는다. 실제 timestamp gap의 OFF p99는269ms,ON은307ms였다. 공급 dt는300ms 근처에서 잘리므로 ON의300ms를 실제 전체 gap 상한으로 보지 않는다.

아래 표는 고지대 구간의 측정 범위별 wall초당 경과 ms다. 부모/자식과 다른 호출자가 중첩된다. 열을 더해 전체 CPU나 renderer 잔여 시간을 계산하면 안 된다.

|측정 범위|OFF ms/s|ON ms/s|
|---|---:|---:|
|전체 Body.Drive|559.396|562.676|
|그 안의 Traversal.Drive|420.594|417.332|
|그 안의 Contact consumption|129.557|136.289|
|별도 활성 frame-fall|2.603|19.308|
|Traversal의 FOLLOW|400.880|398.107|
|접촉 전체 SolveContacts|198.066|206.287|
|접촉 안의 발 지지|113.532|119.570|
|접촉 안의 본 조회|6.031|5.696|
|접촉 안의 world 검사|69.162|72.511|
|native 경로 query 범위|4.099|3.637|
|인식 범위|66.367|62.346|
|legacy trace 기록|93.691|92.883|
|Mission.OnUpdate|214.745|224.710|
|script PrePhys|565.297|568.554|
|script PreAnim|8.039|7.525|

FOLLOW 호출은 전 개체 합계365.90/347.27회/초였고 평균1.096/1.146ms/호출이었다. 고지대 Mission.OnUpdate는6.339/6.023회/초였다. 이는 실제 AI 인식 함수의 호출 빈도가 아니다. combined handler/preAnim/prePhys/postPhys 호출은12982.8/12335.1회/초이며 per-type 빈도로 분해할 카운터는 없다.

PreAnim 종료→PrePhys 시작의 평균 envelope는50.57/52.16ms였다. native 작업과 스케줄 대기가 포함되고 많은 개체의 구간이 서로 겹치므로 native graph self-time이 아니다. 실제 graph 평가·renderer upload·GPU 프레임·할당/GC self-time은 현재 계측에 없다. GPU 전역 메모리 표본은 약8/16GB였지만 다른 앱이 포함되며 ON 표본은 종료 근처/이후라 위상 비교 자료로 쓰지 않는다.

별도 상태 진단: ON의17번은72.937~179.604초 FALL이었고101.2~179.604초 위치가5mm 범위였다. FOOT_X_SUPPORT 표본194회,현재 LANDING_SPACE_BLOCKED/NO_VALID_LOCAL_EXIT였다. 이는 성공한 착지 후 owner24회와 별개인 착지 전 정체다. 180초 끝의 상태는OFF FOLLOW64명,ON FOLLOW63명+FALL1명이었다. 나머지 미도착을 전부 정체로 분류하지 않는다.

새8명 시험과 새로운300초 자연 입력 회귀는 없다. 이전8명8/8·고지대0,64명64/64·고지대28·p95 181ms라는 기록은 그대로 보존하며 이번 비용 자극의19/13 도착으로 덮어쓰지 않는다. 이번 두 비용 진단도 전체 기능 수용 기준을 통과한 것은 아니다.

4. 원인 분석

관찰 사실: 복구 OFF에서도 고지대 Body.Drive 약559ms/s,FOLLOW 약401ms/s가 남았다. ON도 각각563/398ms/s였다. 착지 대기만이 무거운 비용의 필요조건이라는 설명은 맞지 않는다. contact의 발 지지 범위113.5/119.6ms/s는 본 조회6.0/5.7ms/s보다 컸다. native nav query 범위는4.1/3.6ms/s였으나 이것이 전체 경로·접근 비용은 아니다. PrePhys script 범위에 실제 gameplay Drive가 중첩된다는 소스 관계와 측정 크기도 맞았다.

추정: FOLLOW 내부의 지면 실행·입구 정렬·반복 접촉/지지 검사가 다음 비용 분해 우선순위다. 실제 인과적 최적화 효과는 미측정이다. 17번은 착지 footprint 거부 이후 중력 진행 전에 반환하는 경로와 관계있을 수 있지만 실제 같은 입력의 분기 재현 전까지 원인을 확정하지 않는다.

반증/제외: CPU 본 조회만 줄이면 대부분 해결된다는 근거는 없다. 부모/자식 scope 합산,그래프 gap 합계의 CPU 해석,한 쌍의 p95 차이로 회귀 확정,일치하는 player target을 일치하는 NPC replay로 해석하는 것은 제외한다. GPU/renderer를 미측정 잔여 시간의 원인으로 지정하지 않는다.

5. 검증 수준

진단 성공: source 가역 변환·실제 target/config 일치·전체 이동/상태/callback scope·clock3개 검사·정상 종료 및 개체별 정합성 검사. 각 조건1회라 반복 검증 완료/최적화 성공/시각 품질 수용/20ms p95/최종 수용은 아니다. owner ON의24회 안전 인계 journal은 실제 EGRESS→FOLLOW와 연결됐으나17번의 착지 전 정체는 미해결이다. 기존 자연 입력 기능 실패를 비용 실험으로 지우지 않는다.

6. 변경 사항

stage_horde_fixed_stimulus_cost.py는 별도 미션의 player 자극만 바꾼다. launcher에 FixedStimulusCost selector/provenance와 기존 RuntimeTimingProbe 활성화를 추가했다. compiler에 새 validation을 허용했다. 새 callback/고정 자극 비교 분석기와17번 파생 증거를 추가했다. NPC/PBO·native graph·애니메이션/보간·충돌/착지 gate·운영 서버는 변경하지 않았다.

F6DEBD09DB937853C6E7CF81AFB95C842AC1762C02EEFD13816FA24FAA378FE8은 그대로다. 이번에는 재빌드/재서명하지 않았으며 새 엔진 컴파일과 두 실행 전에 기존 base453D/render0C6E/integrationF6DE의 서명 검증을 성공했다. 개인 키는 배포하지 않았다. 두180초 실행 모두 정상 종료했고 raw/이전 보고서를 덮어쓰지 않았다.

7. 원시 증거

runtime-20261029-171420 OFF와171916 ON의 native-drive-cost,ground,pose-cadence,player/state/landing/backoff TSV와 launch/config/compile/분석을 연결했다. 이전164705/165343 기록의 post-hoc legacy scopes와 입력 해시도 연결한다. fixed-pair-analysis.json,fall17-diagnosis.json,source coverage,staged source manifest,clock 검사 결과,관측/전역GPU 표본의 한계를 보존한다. 공개 패킷은 고지대 집계·실제 player target·17번 선정 표본·분석 함수만 제공하며 내부 전체 로그·로컬 경로·프로세스 정보·게임 자산은 공개하지 않는다.

8. 잔여 문제

우선17번의 마지막 실제 touchdown 질의/4joint pose를 재현해 물리 착지 전 반환·압력 탈출·기하학적 support가 어떻게 연결되는지 확인한다. 실제 world/peer/fixture non-deepening gate를 완화하거나 root를 강제로 내려서 성공시키지 않는다. FOLLOW는 지면 실행·입구 정렬·surface approach·contact·nav call을 세분화하고 실제 AI 호출 빈도/계측 오버헤드를 확보한다. 동일한 실제 player 자극에서 순서 교차 반복 후에만 후보 최적화를 평가한다.

애니메이션은 달리기/등반의 정확한 HUD/CPU/영상 frame join을 별도 진행한다. graph/upload/GPU/allocation/GC self-time을 얻을 도구/계측은 아직 미실행이며 미측정을 성공으로 보고하지 않는다. 원인 전 새 보간/배속은 도입하지 않는다. active combat 후 final guard 소유권과 실제 EGRESS 무효화도 이전 미검증 항목으로 유지한다. 목표는 그대로이며 골은 활성 상태다.

원본 보고서 ID: 2026-10-29_173258_249618+0900_npc-fixed-stimulus-cost.md
원본 SHA-256: 97b0665e23a6311e6073be9a105875c88314de72b2f29940691a3ff673280d1a
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selected diagnostic projection;not complete raw logs
{
  "runs": [
    {
      "runId": "runtime-20261029-171420",
      "ownerEnabled": false,
      "actualStimulus": {
        "GROUND_EAST": {
          "samples": 60,
          "requestedXZ": [
            4563,
            10243
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.238617, 10243.000000>",
          "lastPlayer": "<4563.000000, 339.238617, 10243.000000>",
          "placementRows": 1,
          "zeroMovementInput": true,
          "targetMatched": true
        },
        "GROUND_NORTH": {
          "samples": 67,
          "requestedXZ": [
            4563,
            10283
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "lastPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "placementRows": 1,
          "zeroMovementInput": true,
          "targetMatched": true
        },
        "FINAL_HOLD": {
          "samples": 60,
          "requestedXZ": [
            4563,
            10283
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "lastPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "placementRows": 0,
          "zeroMovementInput": true,
          "targetMatched": true
        }
      },
      "highHoldCost": {
        "wholeBodyMsPerSecond": 559.3964370589626,
        "traversalMsPerSecond": 420.59398917498686,
        "contactConsumptionMsPerSecond": 129.55747232501457,
        "activeFrameFallMsPerSecond": 2.6027559316306186,
        "traversalPhases": {
          "follow": {
            "calls": 21515,
            "callsPerSecond": 365.90136054421765,
            "elapsedEquivalentMsPerWallSecond": 400.88025015063715,
            "elapsedEquivalentMeanMsPerInvocation": 1.0955965005278858
          },
          "climb": {
            "calls": 1517,
            "callsPerSecond": 25.799319727891156,
            "elapsedEquivalentMsPerWallSecond": 11.956253492205308,
            "elapsedEquivalentMeanMsPerInvocation": 0.46343289739068694
          },
          "drop": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          },
          "egress": {
            "calls": 112,
            "callsPerSecond": 1.9047619047619047,
            "elapsedEquivalentMsPerWallSecond": 3.0295939452481595,
            "elapsedEquivalentMeanMsPerInvocation": 1.590536821255284
          },
          "landSettle": {
            "calls": 610,
            "callsPerSecond": 10.374149659863944,
            "elapsedEquivalentMsPerWallSecond": 4.727808253645247,
            "elapsedEquivalentMeanMsPerInvocation": 0.4557297136300663
          },
          "fall": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          },
          "other": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          }
        },
        "legacyScopes": {
          "seconds": 59.312999999999995,
          "intervals": 105,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 70.09300816380107,
            "perceptionCpuTicks": 66.36684105753214,
            "socialCpuTicks": 2.9048357873646067,
            "pursuitCpuTicks": 0.49235204914327096,
            "crowdCpuTicks": 11.847990864530253,
            "staticCpuTicks": 2.2797338722379186,
            "commitTicks": 7.879099449361807,
            "traceTicks": 93.69060545983355,
            "missionUpdateTicks": 214.74476066500242,
            "driveTicks": 287.5392391379387,
            "navTicks": 4.099238988193243,
            "bypassTicks": 0.0,
            "contactCpuTicks": 198.06618854469465,
            "contactBoneCpuTicks": 6.030733101510666,
            "contactFootCpuTicks": 113.5319842290495,
            "contactWorldCpuTicks": 69.16170408780026,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.8509292539741146
          },
          "missionUpdatesPerSecond": 6.339251091666246
        },
        "callbackScopes": {
          "seconds": 59.312999999999995,
          "intervals": 105,
          "scriptCallbackElapsedEquivalentMsPerWallSecond": {
            "partHandlerElapsedTicks": 1.1374427275052805,
            "partPreAnimElapsedTicks": 8.039186566304489,
            "partPrePhysElapsedTicks": 565.297379813271,
            "partPostPhysElapsedTicks": 0.7870838933223402
          },
          "missionOnUpdateElapsedEquivalentMsPerWallSecond": 214.74476066500242,
          "missionOnUpdateCallsPerSecond": 6.339251091666246,
          "combinedScriptCallbackCallsPerSecond": 12982.786235732472,
          "preAnimEndToPrePhysStartEnvelope": {
            "samples": 192512,
            "aggregateElapsedEquivalentMsPerWallSecond": 164134.54226484915,
            "meanElapsedMsPerSample": 50.56989748875393,
            "cpuSelfTime": false,
            "nativeGraphSelfTime": false
          }
        }
      },
      "clock": {
        "observedMissionSeconds": 174.87800000000036,
        "nativeAdvanceSeconds": 75.05009999999999,
        "calls": 1501,
        "nativeClockPerMissionSecond": 0.42915689795171397,
        "callbackCallsPerMissionSecond": 8.583126522489946,
        "nativeAdvancePerCallback": 0.05000006662225182
      },
      "foregroundMissionFrameMs": {
        "samples": 1502,
        "p50": 97.0,
        "p95": 219.0,
        "p99": 267.0
      },
      "terminalArrivalsAt180Seconds": 19,
      "highMisses": [
        0,
        4,
        7,
        9,
        11,
        12,
        15,
        16,
        19,
        20,
        24,
        26,
        27,
        28,
        33,
        35,
        36,
        39,
        43,
        44,
        47,
        48,
        50,
        51,
        52,
        53,
        55,
        56,
        59,
        60,
        61,
        62,
        63
      ],
      "admissionViolations": [],
      "egressViolations": []
    },
    {
      "runId": "runtime-20261029-171916",
      "ownerEnabled": true,
      "actualStimulus": {
        "GROUND_EAST": {
          "samples": 55,
          "requestedXZ": [
            4563,
            10243
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.238617, 10243.000000>",
          "lastPlayer": "<4563.000000, 339.238617, 10243.000000>",
          "placementRows": 1,
          "zeroMovementInput": true,
          "targetMatched": true
        },
        "GROUND_NORTH": {
          "samples": 58,
          "requestedXZ": [
            4563,
            10283
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "lastPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "placementRows": 1,
          "zeroMovementInput": true,
          "targetMatched": true
        },
        "FINAL_HOLD": {
          "samples": 60,
          "requestedXZ": [
            4563,
            10283
          ],
          "maximumXZErrorMeters": 0.0,
          "firstPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "lastPlayer": "<4563.000000, 339.408661, 10283.000000>",
          "placementRows": 0,
          "zeroMovementInput": true,
          "targetMatched": true
        }
      },
      "highHoldCost": {
        "wholeBodyMsPerSecond": 562.6757007362971,
        "traversalMsPerSecond": 417.33163642367754,
        "contactConsumptionMsPerSecond": 136.28856967046468,
        "activeFrameFallMsPerSecond": 19.308099645953973,
        "traversalPhases": {
          "follow": {
            "calls": 20335,
            "callsPerSecond": 347.27440398934357,
            "elapsedEquivalentMsPerWallSecond": 398.10660914824354,
            "elapsedEquivalentMeanMsPerInvocation": 1.1463747531489819
          },
          "climb": {
            "calls": 1442,
            "callsPerSecond": 24.625999043650523,
            "elapsedEquivalentMsPerWallSecond": 12.332250859336401,
            "elapsedEquivalentMeanMsPerInvocation": 0.500781748487727
          },
          "drop": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          },
          "egress": {
            "calls": 39,
            "callsPerSecond": 0.6660291003483845,
            "elapsedEquivalentMsPerWallSecond": 1.8381119648262252,
            "elapsedEquivalentMeanMsPerInvocation": 2.759807287496524
          },
          "landSettle": {
            "calls": 583,
            "callsPerSecond": 9.956281166746363,
            "elapsedEquivalentMsPerWallSecond": 5.0546661590332995,
            "elapsedEquivalentMeanMsPerInvocation": 0.5076861605632141
          },
          "fall": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          },
          "other": {
            "calls": 0,
            "callsPerSecond": 0.0,
            "elapsedEquivalentMsPerWallSecond": 0.0,
            "elapsedEquivalentMeanMsPerInvocation": null
          }
        },
        "legacyScopes": {
          "seconds": 59.107000000000006,
          "intervals": 105,
          "elapsedEquivalentMsPerWallSecond": {
            "senseTicks": 66.29889542327895,
            "perceptionCpuTicks": 62.345532132405154,
            "socialCpuTicks": 3.1470761249431387,
            "pursuitCpuTicks": 0.490877492686759,
            "crowdCpuTicks": 10.876122127102192,
            "staticCpuTicks": 2.1812942904072394,
            "commitTicks": 7.463937712838658,
            "traceTicks": 92.8828063417817,
            "missionUpdateTicks": 224.71019124770717,
            "driveTicks": 282.5013910124531,
            "navTicks": 3.6368759966720967,
            "bypassTicks": 0.0,
            "contactCpuTicks": 206.2869329796585,
            "contactBoneCpuTicks": 5.696101377421421,
            "contactFootCpuTicks": 119.56992526561186,
            "contactWorldCpuTicks": 72.51083720524478,
            "contactFallbackCpuTicks": 0.0,
            "contactDiagnosticCpuTicks": 0.4541301105119288
          },
          "missionUpdatesPerSecond": 6.0229752821154845
        },
        "callbackScopes": {
          "seconds": 59.107000000000006,
          "intervals": 105,
          "scriptCallbackElapsedEquivalentMsPerWallSecond": {
            "partHandlerElapsedTicks": 1.0700471602693178,
            "partPreAnimElapsedTicks": 7.524917187006502,
            "partPrePhysElapsedTicks": 568.5536970267136,
            "partPostPhysElapsedTicks": 0.7549656792794401
          },
          "missionOnUpdateElapsedEquivalentMsPerWallSecond": 224.71019124770717,
          "missionOnUpdateCallsPerSecond": 6.0229752821154845,
          "combinedScriptCallbackCallsPerSecond": 12335.053377772512,
          "preAnimEndToPrePhysStartEnvelope": {
            "samples": 182272,
            "aggregateElapsedEquivalentMsPerWallSecond": 160840.5985726936,
            "meanElapsedMsPerSample": 52.157244446959496,
            "cpuSelfTime": false,
            "nativeGraphSelfTime": false
          }
        }
      },
      "clock": {
        "observedMissionSeconds": 174.8889999999999,
        "nativeAdvanceSeconds": 68.19969999999999,
        "calls": 1364,
        "nativeClockPerMissionSecond": 0.38995991743334363,
        "callbackCallsPerMissionSecond": 7.799232656141901,
        "nativeAdvancePerCallback": 0.04999978005865102
      },
      "foregroundMissionFrameMs": {
        "samples": 1365,
        "p50": 106.0,
        "p95": 241.0,
        "p99": 300.0
      },
      "terminalArrivalsAt180Seconds": 13,
      "highMisses": [
        3,
        4,
        7,
        8,
        9,
        10,
        12,
        13,
        16,
        17,
        18,
        19,
        22,
        23,
        26,
        27,
        30,
        31,
        34,
        36,
        38,
        39,
        41,
        43,
        46,
        48,
        49,
        50,
        51,
        52,
        54,
        55,
        56,
        58,
        59,
        60,
        61,
        62
      ],
      "admissionViolations": [],
      "egressViolations": []
    }
  ],
  "observedDifferences": {
    "GROUND_NORTH": {
      "wholeBodyMsPerSecond": {
        "off": 404.6273990599027,
        "on": 416.70790978786005,
        "absoluteObservedDifference": 12.080510727957346,
        "relativeObservedDifference": 0.029855889037729,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 234.88634195622697,
        "on": 239.8067363746686,
        "absoluteObservedDifference": 4.920394418441617,
        "relativeObservedDifference": 0.020947980105878502,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 157.1417559665191,
        "on": 164.73710179544608,
        "absoluteObservedDifference": 7.59534582892698,
        "relativeObservedDifference": 0.04833435761367754,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 0.0,
        "on": 39.40298141372759,
        "absoluteObservedDifference": 39.40298141372759,
        "relativeObservedDifference": null,
        "causalOptimizationClaim": false
      }
    },
    "HIGH_HOLD": {
      "wholeBodyMsPerSecond": {
        "off": 559.3964370589626,
        "on": 562.6757007362971,
        "absoluteObservedDifference": 3.279263677334484,
        "relativeObservedDifference": 0.005862146163417262,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 420.59398917498686,
        "on": 417.33163642367754,
        "absoluteObservedDifference": -3.2623527513093222,
        "relativeObservedDifference": -0.00775653679147571,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 129.55747232501457,
        "on": 136.28856967046468,
        "absoluteObservedDifference": 6.7310973454501095,
        "relativeObservedDifference": 0.051954528169275566,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 2.6027559316306186,
        "on": 19.308099645953973,
        "absoluteObservedDifference": 16.705343714323355,
        "relativeObservedDifference": 6.41832893791832,
        "causalOptimizationClaim": false
      }
    },
    "FINAL_HOLD": {
      "wholeBodyMsPerSecond": {
        "off": 362.3909855833443,
        "on": 379.17567120300606,
        "absoluteObservedDifference": 16.784685619661786,
        "relativeObservedDifference": 0.04631650975711583,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 208.11643897222064,
        "on": 217.02106460246617,
        "absoluteObservedDifference": 8.904625630245533,
        "relativeObservedDifference": 0.04278674800616842,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 142.78756361744945,
        "on": 151.25616860843982,
        "absoluteObservedDifference": 8.468604990990372,
        "relativeObservedDifference": 0.059309121722106756,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 0.0,
        "on": 39.440155479281835,
        "absoluteObservedDifference": 39.440155479281835,
        "relativeObservedDifference": null,
        "causalOptimizationClaim": false
      }
    },
    "GROUND_EAST_HOLD": {
      "wholeBodyMsPerSecond": {
        "off": 385.7298533185165,
        "on": 385.55488401497354,
        "absoluteObservedDifference": -0.17496930354298001,
        "relativeObservedDifference": -0.00045360581255944187,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 221.3632741198806,
        "on": 221.84942950383683,
        "absoluteObservedDifference": 0.4861553839562305,
        "relativeObservedDifference": 0.0021961880799294202,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 152.4003354236913,
        "on": 152.63842327639176,
        "absoluteObservedDifference": 0.23808785270045973,
        "relativeObservedDifference": 0.0015622528128861842,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 0.0,
        "on": 41.62986737875559,
        "absoluteObservedDifference": 41.62986737875559,
        "relativeObservedDifference": null,
        "causalOptimizationClaim": false
      }
    },
    "LOW_HOLD": {
      "wholeBodyMsPerSecond": {
        "off": 509.1426975701704,
        "on": 529.1707377594221,
        "absoluteObservedDifference": 20.02804018925167,
        "relativeObservedDifference": 0.03933679160053433,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 369.90151191983915,
        "on": 384.75983946561985,
        "absoluteObservedDifference": 14.858327545780696,
        "relativeObservedDifference": 0.040168334183507265,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 128.26915006371678,
        "on": 133.67243932758126,
        "absoluteObservedDifference": 5.40328926386448,
        "relativeObservedDifference": 0.04212462046548554,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 1.2505533797056703,
        "on": 1.6404410878365225,
        "absoluteObservedDifference": 0.3898877081308523,
        "relativeObservedDifference": 0.3117721438029427,
        "causalOptimizationClaim": false
      }
    },
    "GROUND_EAST": {
      "wholeBodyMsPerSecond": {
        "off": 400.0103013997727,
        "on": 415.93952977317747,
        "absoluteObservedDifference": 15.929228373404783,
        "relativeObservedDifference": 0.039822045376488986,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 232.73231123796515,
        "on": 242.60462087808898,
        "absoluteObservedDifference": 9.87230964012383,
        "relativeObservedDifference": 0.04241916211638335,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 154.7878986487867,
        "on": 161.15559378226476,
        "absoluteObservedDifference": 6.367695133478065,
        "relativeObservedDifference": 0.04113819742411742,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 0.0,
        "on": 39.75881694221491,
        "absoluteObservedDifference": 39.75881694221491,
        "relativeObservedDifference": null,
        "causalOptimizationClaim": false
      }
    },
    "GROUND_HOLD": {
      "wholeBodyMsPerSecond": {
        "off": 396.8611564391397,
        "on": 397.69310851163783,
        "absoluteObservedDifference": 0.8319520724981544,
        "relativeObservedDifference": 0.0020963303135103817,
        "causalOptimizationClaim": false
      },
      "traversalMsPerSecond": {
        "off": 253.31142452612775,
        "on": 254.72604607627537,
        "absoluteObservedDifference": 1.414621550147615,
        "relativeObservedDifference": 0.005584515395600344,
        "causalOptimizationClaim": false
      },
      "contactConsumptionMsPerSecond": {
        "off": 131.5565950878681,
        "on": 132.10877028341577,
        "absoluteObservedDifference": 0.5521751955476759,
        "relativeObservedDifference": 0.0041972445028610845,
        "causalOptimizationClaim": false
      },
      "activeFrameFallMsPerSecond": {
        "off": 9.952707752714998,
        "on": 56.87236039801356,
        "absoluteObservedDifference": 46.91965264529856,
        "relativeObservedDifference": 4.714260059781154,
        "causalOptimizationClaim": false
      }
    }
  },
  "fall17": {
    "sourceRunId": "runtime-20261029-171916",
    "id": 17,
    "continuousFallSampleSeconds": [
      72.937,
      179.604
    ],
    "stationarySuffixSeconds": [
      101.2,
      179.604
    ],
    "stationaryDistanceToleranceMeters": 0.005,
    "first": {
      "seconds": "72.937",
      "position": "<4511.428711, 339.799561, 10239.486328>",
      "heading": "178.696",
      "reason": "NATIVE_FALL",
      "traversalPhase": "FALL",
      "area": "low",
      "landingDiagnostic": "CLEAR",
      "pressureEscapeDiagnostic": "NONDEEPENING_CLEAR",
      "pressureEscapePlans": "1",
      "pressureEscapeRejected": "0",
      "pressureEscapeSteps": "7",
      "pressureEscapeLastStepReject": "DEEPER_FIXTURE",
      "pressureEscapeRejectNormal": "<0.000000, 0.000000, 0.000000>",
      "separationEpisodeDistance": "0.201172",
      "nativeSeconds": "25.8999",
      "pressureEscapeRejectProbe": "0",
      "pressureEscapeRejectStart": "<4511.403809, 340.336273, 10239.695313>",
      "pressureEscapeRejectEnd": "<4511.374023, 340.336273, 10239.695313>"
    },
    "last": {
      "seconds": "179.604",
      "position": "<4511.493164, 339.275757, 10236.865234>",
      "heading": "166.976",
      "reason": "LANDING_SPACE_BLOCKED",
      "traversalPhase": "FALL",
      "area": "ground",
      "landingDiagnostic": "FOOT_X_SUPPORT",
      "pressureEscapeDiagnostic": "NO_VALID_LOCAL_EXIT",
      "pressureEscapePlans": "596",
      "pressureEscapeRejected": "4",
      "pressureEscapeSteps": "107",
      "pressureEscapeLastStepReject": "NEW_WORLD_CONTACT",
      "pressureEscapeRejectNormal": "<0.999249, 0.018560, -0.034028>",
      "separationEpisodeDistance": "2.81813",
      "nativeSeconds": "70.1497",
      "pressureEscapeRejectProbe": "0",
      "pressureEscapeRejectStart": "<4511.493164, 339.625763, 10236.865234>",
      "pressureEscapeRejectEnd": "<4511.464844, 339.625763, 10236.865234>"
    },
    "lastTransitions": [
      {
        "sequence": "120",
        "seconds": "27.29",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "MOTION",
        "next": "SETTLE",
        "reason": "NATIVE_MOTION",
        "failure": "",
        "recoveryGeneration": "0",
        "position": "<4509.570313, 340.165619, 10237.253906>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0429993",
        "terminalOutcome": "NONTERMINAL",
        "detail": "CLEAR",
        "tested": "<0.000000, 0.000000, 0.000000>",
        "supported": "<0.000000, 0.000000, 0.000000>",
        "valid": "false",
        "motionElapsedBeforeTransition": "1.5",
        "clipDuration": "1.48333",
        "nativeCurveEnded": "true",
        "endpointError": "2.94425e-05",
        "exitError": "0.0142242",
        "nativeSeconds": "11.8",
        "link": "ground_to_low_lane_1",
        "callContext": "MOTION",
        "areaAtCall": "low"
      },
      {
        "sequence": "140",
        "seconds": "28.604",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "SETTLE",
        "next": "EGRESS",
        "reason": "SETTLE",
        "failure": "",
        "recoveryGeneration": "0",
        "position": "<4509.570313, 340.178223, 10237.253906>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0327759",
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
        "nativeSeconds": "12.3",
        "link": "ground_to_low_lane_1",
        "callContext": "OTHER",
        "areaAtCall": "low"
      },
      {
        "sequence": "142",
        "seconds": "28.733",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "EGRESS",
        "next": "FOLLOW",
        "reason": "SUPPORTED_EGRESS_HANDOFF",
        "failure": "",
        "recoveryGeneration": "0",
        "position": "<4509.570313, 340.178223, 10237.253906>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0324707",
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
        "nativeSeconds": "12.35",
        "link": "none",
        "callContext": "OTHER",
        "areaAtCall": "low"
      },
      {
        "sequence": "460",
        "seconds": "72.382",
        "id": "17",
        "kind": "TRANSITION",
        "previous": "FOLLOW",
        "next": "FALL",
        "reason": "ALIGN_HEADING",
        "failure": "CONTACT_SUPPORT_LOST",
        "recoveryGeneration": "1",
        "position": "<4511.321289, 340.180634, 10239.720703>",
        "contactStatus": "",
        "contactBlocked": "false",
        "lift": "0.0562744",
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
        "nativeSeconds": "25.7999",
        "link": "none",
        "callContext": "PRESSURE_SUPPORT_LOSS",
        "areaAtCall": "low"
      }
    ],
    "firstLandingDetail": [
      {
        "sequence": "200",
        "seconds": "73.334",
        "id": "17",
        "generation": "1",
        "phase": "FALL",
        "gate": "FOOT_X_SUPPORT",
        "valid": "false",
        "requireEntitySpace": "false",
        "root": "<4511.466309, 339.380249, 10239.336914>",
        "tested": "<4511.484375, 339.179260, 10239.331055>",
        "supported": "<4511.484375, 339.179260, 10239.331055>",
        "heading": "178.696",
        "lift": "0",
        "reason": "NATIVE_FALL",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "1",
        "rejected": "1",
        "budgetSkipped": "0",
        "poseAvailable": "true",
        "nativeBefore": "25.9999",
        "nativeAfter": "25.9999",
        "updatesBefore": "520",
        "updatesAfter": "520",
        "hipsMS": "<-0.010169, 0.979330, -0.003902>",
        "headMS": "<-0.038455, 1.638029, -0.213963>",
        "leftFootMS": "<0.168402, 0.422690, -0.216403>",
        "rightFootMS": "<-0.185973, 0.160976, -0.235643>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      }
    ],
    "lastLandingDetail": [
      {
        "sequence": "472",
        "seconds": "179.773",
        "id": "17",
        "generation": "1",
        "phase": "FALL",
        "gate": "FOOT_X_SUPPORT",
        "valid": "false",
        "requireEntitySpace": "false",
        "root": "<4511.493164, 339.275757, 10236.865234>",
        "tested": "<4511.501953, 339.172150, 10236.862305>",
        "supported": "<4511.501953, 339.172150, 10236.862305>",
        "heading": "166.976",
        "lift": "0",
        "reason": "FRAME_FALL_PENDING",
        "failure": "CONTACT_SUPPORT_LOST",
        "checks": "2533",
        "rejected": "2533",
        "budgetSkipped": "0",
        "poseAvailable": "true",
        "nativeBefore": "70.1997",
        "nativeAfter": "70.1997",
        "updatesBefore": "1404",
        "updatesAfter": "1404",
        "hipsMS": "<-0.012731, 0.978220, 0.001843>",
        "headMS": "<-0.054262, 1.633673, -0.210579>",
        "leftFootMS": "<0.168536, 0.250544, -0.314792>",
        "rightFootMS": "<-0.122935, 0.244351, 0.059489>",
        "idleOffset": "<0.000000, 0.000000, 0.000000>",
        "link": "none"
      }
    ],
    "sampledLandingReasons": {
      "CLEAR": 1,
      "FOOT_X_SUPPORT": 194
    },
    "limitations": [
      "Sampled5mm stationary suffix is not proof of zero subframe motion.",
      "Player-placement cost experiment180s;this is unresolved FALL ownership,not failed native climb completion.",
      "Post-touchdown owner cannot intervene before physical LAND. No collision gate changed."
    ]
  },
  "previousNaturalRuns": [
    {
      "runId": "runtime-20261029-164705",
      "count": 8,
      "highHold": {
        "seconds": 59.358999999999995,
        "intervals": 116,
        "elapsedEquivalentMsPerWallSecond": {
          "senseTicks": 5.990996626033759,
          "perceptionCpuTicks": 2.7892372452204213,
          "socialCpuTicks": 2.611354236696486,
          "pursuitCpuTicks": 0.2030989652583447,
          "crowdCpuTicks": 9.00496721093685,
          "staticCpuTicks": 1.0256937441211003,
          "commitTicks": 9.515940743614138,
          "traceTicks": 17.485247775365696,
          "missionUpdateTicks": null,
          "driveTicks": 35.13131297656429,
          "navTicks": 0.0,
          "bypassTicks": 0.0,
          "contactCpuTicks": 53.391873927258004,
          "contactBoneCpuTicks": 1.9110588695848771,
          "contactFootCpuTicks": 32.13056695475347,
          "contactWorldCpuTicks": 16.60379779444032,
          "contactFallbackCpuTicks": 0.0,
          "contactDiagnosticCpuTicks": 0.09225522599476993
        },
        "missionUpdatesPerSecond": null
      }
    },
    {
      "runId": "runtime-20261029-165343",
      "count": 64,
      "highHold": {
        "seconds": 59.239,
        "intervals": 107,
        "elapsedEquivalentMsPerWallSecond": {
          "senseTicks": 80.42116381733318,
          "perceptionCpuTicks": 75.59303782817373,
          "socialCpuTicks": 3.777828810481377,
          "pursuitCpuTicks": 0.6032777353016427,
          "crowdCpuTicks": 15.636311907853932,
          "staticCpuTicks": 2.7020929007157526,
          "commitTicks": 11.07444013597654,
          "traceTicks": 102.66629601009664,
          "missionUpdateTicks": null,
          "driveTicks": 225.0828227715919,
          "navTicks": 4.119852296747908,
          "bypassTicks": 0.0,
          "contactCpuTicks": 181.44612150015575,
          "contactBoneCpuTicks": 7.561494856005937,
          "contactFootCpuTicks": 103.1044431262935,
          "contactWorldCpuTicks": 59.93924966398607,
          "contactFallbackCpuTicks": 0.0,
          "contactDiagnosticCpuTicks": 0.9859643664366653
        },
        "missionUpdatesPerSecond": null
      }
    }
  ],
  "scopeLimits": [
    "NPC trajectories,phase populations and call counts are not identical;actual target matching is not deterministic NPC replay.",
    "One sequential pair does not establish causal regression or reproducible optimization.",
    "Elapsed scopes overlap/nest;never sum Body/Traversal/Contact/PrePhys or call a remainder renderer time.",
    "No graph/render/GPU self-time or allocation/GC telemetry;mission frequency is not AI-call frequency.",
    "Other user applications remain running;instrumentation and fixed camera are diagnostic conditions."
  ]
}

Own callback analysis
"""Attribute script callback elapsed scopes; never native graph self-time."""
import argparse
import bisect
import csv
import hashlib
import json
from collections import defaultdict
from pathlib import Path
from analyze_horde_legacy_cost_scopes import ground_clock_calibration

ACTOR=('partHandlerElapsedTicks','partPreAnimElapsedTicks','partPrePhysElapsedTicks',
       'partPostPhysElapsedTicks','partTimingCalls','partNativeGapTicks','partNativeGapCount')

def read(p):
    with p.open(encoding='utf-8-sig') as f:return list(csv.DictReader(f,delimiter='\t'))

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--profile',type=Path,required=True);a=ap.parse_args();p=a.profile
    output=p/'callback-elapsed-analysis.json';assert not output.exists()
    launch=json.loads((p/'launch.json').read_text(encoding='utf-8-sig'))
    assert launch['clientExited'] and launch['normalQuitVerified'] and not launch['typedFailed']
    rows=read(p/'native-ground.tsv');assert rows and all(r['runtimeTimingProbe']=='true' for r in rows)
    expected=set(range(launch['count']));cohorts=defaultdict(dict)
    for r in rows:
        c=cohorts[float(r['seconds'])];assert int(r['id']) not in c;c[int(r['id'])]=r
    stimulus=read(p/'native-natural-player.tsv');times=[float(r['seconds']) for r in stimulus]
    def stage(t):return stimulus[max(0,bisect.bisect_right(times,t)-1)]['stage']
    clock=ground_clock_calibration(rows);rate=clock['ticksPerSecond']
    groups=defaultdict(lambda:dict(seconds=0,intervals=0,deltas=defaultdict(float)))
    rejected=defaultdict(int)
    for t0,t1 in zip(sorted(cohorts),sorted(cohorts)[1:]):
        before,after=cohorts[t0],cohorts[t1]
        assert set(before)==expected and set(after)==expected
        if t0<5 or t1-t0>1.5 or stage(t0)!=stage(t1):rejected['warmup_gap_stage_boundary']+=1;continue
        if any(r['appActive']!='true' for r in [*before.values(),*after.values()]):rejected['background']+=1;continue
        g=groups[stage(t0)];g['seconds']+=t1-t0;g['intervals']+=1
        for key in ACTOR:
            d=[float(after[i][key])-float(before[i][key]) for i in expected];assert all(v>=0 for v in d),key
            g['deltas'][key]+=sum(d)
        for key in ['missionUpdateTicks','missionUpdateCalls']:
            d=float(after[0][key])-float(before[0][key]);assert d>=0;g['deltas'][key]+=d
    stages={}
    for name,g in groups.items():
        seconds=g['seconds'];d=g['deltas'];gaps=d['partNativeGapCount']
        stages[name]=dict(seconds=seconds,intervals=g['intervals'],
            scriptCallbackElapsedEquivalentMsPerWallSecond={k:d[k]/rate/seconds*1000 for k in ACTOR[:4]},
            missionOnUpdateElapsedEquivalentMsPerWallSecond=d['missionUpdateTicks']/rate/seconds*1000,
            missionOnUpdateCallsPerSecond=d['missionUpdateCalls']/seconds,
            combinedScriptCallbackCallsPerSecond=d['partTimingCalls']/seconds,
            preAnimEndToPrePhysStartEnvelope=dict(samples=int(gaps),
                aggregateElapsedEquivalentMsPerWallSecond=d['partNativeGapTicks']/rate/seconds*1000,
                meanElapsedMsPerSample=d['partNativeGapTicks']/rate/gaps*1000 if gaps else None,
                cpuSelfTime=False,nativeGraphSelfTime=False))
    result=dict(sourceRunId=p.name,buildId=launch['subsetIntegrationPboSha256'],count=launch['count'],
        clockCalibration=clock,stages=stages,rejectedIntervals=dict(rejected),
        observedPartTimingActorsMinimum=min(int(r['partTimingObservedActors']) for r in rows if float(r['seconds'])>=5),
        measuredActorGroups='Per-body sums of part callback counters;mission counter once from actor0.',
        limitations=['Callback elapsed scopes include delegated gameplay and may nest Body.Drive/contact operations;do not sum them.',
            'PreAnim-to-PrePhys envelope includes native work and scheduling/wait;many hosts overlap. It is not native graph or renderer CPU self-time.',
            'MissionOnUpdate frequency is not AI update frequency;perception call counts were not recorded.',
            'No GPU skinning/upload timestamps,allocation or GC self-time.','Combined callback count includes handler/preAnim/prePhys/postPhys;no invented per-type frequency.'],
        inputHashes={n:hashlib.sha256((p/n).read_bytes()).hexdigest() for n in ['launch.json','native-ground.tsv','native-natural-player.tsv']},
        optimizationAccepted=False,visibleSmoothnessAccepted=False,fullGoalComplete=False)
    with output.open('x',encoding='utf-8') as f:json.dump(result,f,indent=2)
    print(json.dumps(dict(sourceRunId=p.name,stages=stages),indent=2))

if __name__=='__main__':main()

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
