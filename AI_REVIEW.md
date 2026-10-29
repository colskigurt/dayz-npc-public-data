# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T105329-previous-contacts-capture-20261029

실제 접촉 이력 확보 — 8→64 기록 연결과 착지 거부 분리

작성: 2026-10-29T19:48:18.541661+09:00

1. 작업 목표

기존 고정 포즈 재현에서 빠졌던 실제 이전 Contacts4점과 같은 Fall 호출의dt/수직 및압력속도를 확보한다. 착지 질의 허용과 실제 LAND 전환을 구분하고,기존 정상 낙하/복구·추적 동작을 보존한다. NPC 이동/애니메이션 방식이나 성능 최적화는 변경하지 않는다.

2. 실제 수행

기존F6DE 빌드·원본9script를 먼저 보존했다. opt-in previous-contact-detail.flag 계측 파일1개를 추가했다. 원래 bounded LDObserveLanding이 실제로 행을 쓴 경우만 같은 sequence의companion 행을 쓴다. 실제 cached4world contacts·Fall dt/후중력속도·압력속도·root/heading/lift·native요청/clock·directorNow/Born/engineTickTime을 읽으며 추가 물리/본 query와Body 필드 쓰기는 없다. 기존9script는 바이트 동일하다.

PBO10개 script의source/packed 바이트,서명·검증을 통과했고,별도 엔진4case에서 flagOFF/실제Fall/빈 Contacts/중복 제한을 확인했다. 과거compile 기록을 덮어쓰지 않도록 실제 수집용 미션을 정확히 복제했다.8명300초,64명300초 혼합 전경,같은64명300초 전경 실행을 각각 정상 종료했다. 마지막64 실행은 CUA로 대상 DayZ 창만 활성화했고 다른 작업의 프로세스/공유 입력 런타임은 보존했다.

3. 측정 결과

|실행|개체|최종 도착|고지대 미도달|낙하 후 재개|접촉 기록 연결|미션p50/p95/p99 ms|
|---|---:|---:|---:|---:|---:|---|
|runtime-20261029-190552|8|8|0|9|10|22.0/33.0/38.0|
|runtime-20261029-191331|64|64|25|60|84|65.0/115.0/135.0|
|runtime-20261029-193252|64|64|27|55|106|52.0/149.0/195.0|

200/200 companion 행이 기존 LD행의sequence/id/generation/phase/root/heading/lift/native clock과 일치했다. 이는 샘플링된200행의연결이며 모든 physics 호출을 전수 기록했다는 뜻이 아니다. 엔진 proof는기존4행/companion3행이고 OFF1행은추가출력없음,빈1행은UNAVAILABLE,중복observer1행은억제됨을 확인했다. 실제8 dt는0.0159912~0.0410004초,p50 0.0209961초였다.64는최대/p50 0.05초였고 압력속도는0이 아닌 표본이 있었다. 기존0.05초/압력0 정규화 재현과 구분해야 한다.

owner는8명6회/혼합64명44회/전경64명40회의 VALIDATED_FOLLOW가 실제 전환과 연결됐고 위반은검출되지 않았다. 중간helper lease 실패는혼합64 시간2회,전경64 거리2회+시간1회였다. 이들은최종owner미종결과 다르며 모두최종인계로끝났다. CLIMB_EXIT_VALIDATION은이번기록에없다. 고지대 미도달25/27명은최종64/64로지우지 않는다.

혼합64의전경cadence266/3139와미션전경표본163개는전구간성능평가로불충분하다. 전경64는cadence4685/4685전경·미션표본4626개였다. 미션dt는GPU Present/표시애니메이션시간이아니고,다른승인작업과동시실행·상세로그·플레이어실이동거리차이가있다. 전경64의GROUND_EAST/NORTH 첫끝표본직선변위는약38.707/14.636m,8명은39.797/39.805m였다. p95149ms를이전181ms나고정자극219/241ms 대비최적화성공으로해석하지 않는다.20ms목표는미달이다.

전경64에서callback NativeSeconds 누적은관측294.904초에175.606초,비율0.59547이었다.8명비율0.78587이었다. 이값은script에누적된callback pDt이며direct graph phase나실제화면재생배속이아니다.8명초기장면만직접관찰했고64전경전환시로딩화면을확인했다. 연속영상/실제표시프레임측정은없다.

4. 원인 분석

관찰 사실: 혼합64의LandingSpace true66행은물리LAND60회와같지않았다.61개질의행이60개실제전환과호환됐고,동일위치/같은director시각의2개질의가1개전환에대응해호출단위는모호했다. 전경64는허용60행·실제LAND55회이고5개행은직접전환과연결되지않았다. true질의수를물리착지성공수로보고하지 않는다.

혼합64 id4/seq226과전경64 seq152/197/284는허용된발판지점에서관절0 SWEEP 거부기록과연결됐다.컨테이너 충돌의실제normal/fraction·anchor·nativeSeconds·캐시된previous joint가맞았고cached previous차이는0이었다.모델좌표에서재구성한raw와기록된raw의차이는약0.26~0.40mm였으며native bit동일성은아니다. 이4개는실제관절충돌의근거다.나머지미연결행은throttled failure상세가없어원인을확정하지 않는다.

중요한clock관계도확인했다. contact-failure seconds는절대Director.Now,LD seconds는Now-Born이었다. 새companion의directorNow/Born으로연결했으며예:id4는125.318-41.796=83.522다. 서로다른clock의seconds를그대로맞추지 않는다.

추정: 경사면/모서리의전신하강거부를실제입력으로재현한뒤,신선한native normal/fraction을사용하는제한된contact-plane slide를검토할수있다. 아직그후보는구현/시험/배포하지않았다. NativeSeconds비율과사용자가느낀저속재생의관계도표시영상없이확정할수없다. 원본Days Gone전체알고리즘을확보했다는의미가아니다.

5. 검증 수준

진단 성공: 읽기전용source/바이트·서명·엔진4case·실제200행same-call연결·clock변환·물리LAND와질의분리·4개관절거부연결.8명은이번정의된시나리오의부분회귀를통과했다.64명은최종64/64지만고지대25/27과helper실패가남아전체회귀미수용이다. 전경조건/실제자극이다른64실행은통제된반복최적화검증이아니다. 시각품질·20ms·최종수용은미완료다. 동일시각반복질의의정확한Fall호출식별도미완료다.

6. 변경 사항

subset_previous_contact_detail.c를DGSubsetZZPreviousContacts로추가했다. builder에복사1개,launcher에명시적flag/검증·격리미션선택,compiler에별도검증이름을추가했다. 기존9script·base453D·render0C6E는동일하며NPC실행/전투/애니메이션/보간/충돌gate를수정하지않았다.

새integration D61DE1231E4F0DE13FC9DC91097031BE8E2B1EACD85FBD894BB3475BE955116E는빌드·자동서명·서명검증·source/packed10개검증성공이다. 엔진compile두종과3개클라이언트시작전의서명검증도성공했다. build.json의engineCompiled=false는빌드시점보수적메타데이터로유지하고후속실제compile결과를별도보존했다. 개인키는배포하지않았다. 운영서버·이전PBO아카이브·raw를보존했다.

7. 원시 증거

세runtime의launch/compile/config·native-ground/player/state/traversal/pose·landing/previous/contact-failure/owner TSV와분석을연결했다. baseline-F6DE와candidate-D61D의release/source/build/member/signature를보존한다. engine-join-proof와별도previous-contact-proof/live compileprofile도보존했다. 2mm의초기관계분석과엄격좌표분석의모호성기록을덮어쓰지않았고최종outcome-audit에한계를명시했다. 공개패킷은선정표본·집계·새관측/분석함수로제한하며전체내부로그·PBO/게임자산·프로세스/로컬경로는노출하지않는다.

8. 잔여 문제

우선실제id4/seq226과전경3표본을actual4previousContacts/현재MSpose/dt/속도로재현해발판질의와전신SWEEP거부가맞는지검증한다. 그후에만한가지제한된contact-plane응답을검토하며기존root/관절/월드/peer검사와실제LAND조건을우회하지않는다. 정상낙하·밀침·재등반을보존한8→64실기검증이필요하다.

정확한 Fall-call serial/after-result가 없어 중복 질의1건의 호출 배정은 미확정이다. 전체 pre-Fall/peer 이력도 포함하지 않는다. 이는 next-experiment-plan에 남겼다. 고지대 미도달과 helper 거리/시간 한도, 전투 후 owner guard 복귀, CPU-표시 시간 join, 64명 비용 귀속/20ms는 별도 잔여 작업이다. 애니메이션 재생/업데이트 방법을 추측으로 바꾸지 않았다. 모든 실험은 정상 종료했고 골은 활성 상태다.

원본 보고서 ID: 2026-10-29_194818_541661+0900_npc-previous-contacts-capture.md
원본 SHA-256: 8cc510b8e1dec37e487e901247249a68583c866f292fb46a789877a438ca0b1c
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selected run summaries,top3 first/last rejected snapshots,all unmatched-permission evidence;not complete raw logs.
[
  {
    "runId": "runtime-20261029-190552",
    "count": 8,
    "state": {
      "terminalArrivals": 8,
      "highMisses": [],
      "fallsResumed": 9,
      "labels": {
        "CONTACT_SUPPORT_LOST": 1
      },
      "frameMs": {
        "samples": 13772,
        "p50": 22.0,
        "p95": 33.0,
        "p99": 38.0
      },
      "allPhysicalTransitionsMatched": true
    },
    "cadenceAppActive": {
      "true": 13980
    },
    "actualPlayerFirstLastDisplacementMetres": {
      "GROUND_EAST": 39.796932666885596,
      "GROUND_NORTH": 39.80506762432749
    },
    "previousContact": {
      "parentRows": 10,
      "joinedRows": 10,
      "dt": {
        "min": 0.0159912,
        "p50": 0.0209961,
        "max": 0.0410004
      },
      "hist": {
        "0": 0,
        "4": 10
      },
      "selectedSnapshots": [
        {
          "originalLanding": {
            "sequence": "1",
            "seconds": "14.378",
            "id": "0",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4508.706055, 340.161621, 10237.444336>",
            "tested": "<4508.700195, 340.126343, 10237.452148>",
            "supported": "<4508.700195, 340.126312, 10237.452148>",
            "heading": "12",
            "lift": "0.0312195",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "11.3",
            "nativeAfter": "11.3",
            "updatesBefore": "226",
            "updatesAfter": "226",
            "hipsMS": "<0.000000, 0.821200, 0.000000>",
            "headMS": "<-0.068055, 1.153241, -0.607915>",
            "leftFootMS": "<0.204877, 0.096641, -0.402601>",
            "rightFootMS": "<-0.298318, 0.096472, 0.324387>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "1",
            "seconds": "14.378",
            "id": "0",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.0189972",
            "fallVelocity": "-1.18636",
            "pressureVelocity": "<-0.499375, 0.000000, 0.608754>",
            "pressureActive": "true",
            "root": "<4508.706055, 340.161621, 10237.444336>",
            "heading": "12",
            "lift": "0.0312195",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4508.737305, 341.017639, 10237.398438>",
            "previousHead": "<4508.906250, 341.351929, 10237.987305>",
            "previousLeftFoot": "<4508.610840, 340.289917, 10237.845703>",
            "previousRightFoot": "<4508.949707, 340.287628, 10237.028320>",
            "request": "6",
            "nextRequest": "6",
            "nativeRequest": "0",
            "nativeSeconds": "11.3",
            "animUpdates": "226",
            "directorNow": "64.956",
            "directorBorn": "50.578",
            "engineTickTime": "64.961",
            "separationDistance": "0"
          }
        }
      ]
    },
    "owner": {
      "outcomes": {
        "VALIDATED_FOLLOW": 6
      },
      "helperFailures": {},
      "violations": []
    },
    "physicalOutcome": {
      "landingSpaceTrueSamples": 9,
      "physicalFallLandEvents": 9,
      "compatibleQuerySamples": 9,
      "uniquePhysicalEventsCovered": 9,
      "ambiguousQuerySamples": [],
      "unmatchedQuerySamples": [],
      "selectedUnmatchedEvidence": []
    },
    "callbackClock": {
      "observedMissionSeconds": 294.97100000000756,
      "nativeAdvanceSeconds": 231.81,
      "calls": 4636,
      "nativeClockPerMissionSecond": 0.7858738655664254,
      "callbackCallsPerMissionSecond": 15.716799278572745,
      "nativeAdvancePerCallback": 0.05000215703192407
    },
    "clockExcluded": {
      "warmup": 208
    }
  },
  {
    "runId": "runtime-20261029-191331",
    "count": 64,
    "state": {
      "terminalArrivals": 64,
      "highMisses": [
        0,
        1,
        4,
        7,
        11,
        12,
        16,
        18,
        20,
        24,
        25,
        28,
        29,
        31,
        35,
        38,
        42,
        47,
        52,
        54,
        55,
        56,
        58,
        59,
        60
      ],
      "fallsResumed": 60,
      "labels": {
        "CONTACT_SUPPORT_LOST": 13
      },
      "frameMs": {
        "samples": 163,
        "p50": 65.0,
        "p95": 115.0,
        "p99": 135.0
      },
      "allPhysicalTransitionsMatched": true
    },
    "cadenceAppActive": {
      "true": 266,
      "false": 2873
    },
    "actualPlayerFirstLastDisplacementMetres": {
      "GROUND_EAST": 38.283877597829026,
      "GROUND_NORTH": 38.89589386319513
    },
    "previousContact": {
      "parentRows": 84,
      "joinedRows": 84,
      "dt": {
        "min": 0.00299072,
        "p50": 0.05,
        "max": 0.05
      },
      "hist": {
        "0": 0,
        "4": 84
      },
      "selectedSnapshots": [
        {
          "originalLanding": {
            "sequence": "167",
            "seconds": "70.079",
            "id": "18",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.257813, 340.219086, 10241.056641>",
            "tested": "<4511.273438, 340.183990, 10241.062500>",
            "supported": "<4511.273438, 340.183990, 10241.062500>",
            "heading": "-164.386",
            "lift": "0.0248718",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "32.8498",
            "nativeAfter": "32.8498",
            "updatesBefore": "657",
            "updatesAfter": "657",
            "hipsMS": "<0.000000, 0.835120, 0.000000>",
            "headMS": "<-0.129375, 1.134095, -0.613723>",
            "leftFootMS": "<0.207857, 0.097206, -0.395655>",
            "rightFootMS": "<-0.295548, 0.094463, 0.333521>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "167",
            "seconds": "70.079",
            "id": "18",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-1.4905",
            "pressureVelocity": "<0.586038, 0.000000, 0.207913>",
            "pressureActive": "true",
            "root": "<4511.257813, 340.219086, 10241.056641>",
            "heading": "-164.386",
            "lift": "0.0248718",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.234863, 341.088226, 10241.095703>",
            "previousHead": "<4510.941895, 341.357910, 10240.525391>",
            "previousLeftFoot": "<4511.333496, 340.340485, 10240.649414>",
            "previousRightFoot": "<4511.041504, 340.338165, 10241.488281>",
            "request": "6",
            "nextRequest": "6",
            "nativeRequest": "0",
            "nativeSeconds": "32.8498",
            "animUpdates": "657",
            "directorNow": "111.875",
            "directorBorn": "41.796",
            "engineTickTime": "111.879",
            "separationDistance": "0"
          }
        },
        {
          "originalLanding": {
            "sequence": "185",
            "seconds": "73.953",
            "id": "18",
            "generation": "1",
            "phase": "FALL",
            "gate": "WORLD:StaticObj_Container_1C",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.289551, 339.476898, 10243.327148>",
            "tested": "<4511.301270, 339.178009, 10243.331055>",
            "supported": "<4511.301270, 339.177979, 10243.331055>",
            "heading": "-165.386",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "82",
            "rejected": "82",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "34.1998",
            "nativeAfter": "34.1998",
            "updatesBefore": "684",
            "updatesAfter": "684",
            "hipsMS": "<-0.010169, 0.979330, -0.003902>",
            "headMS": "<-0.038455, 1.638029, -0.213963>",
            "leftFootMS": "<0.168402, 0.422690, -0.216403>",
            "rightFootMS": "<-0.185973, 0.160976, -0.235643>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "185",
            "seconds": "73.953",
            "id": "18",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.0359955",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.586038, 0.000000, 0.207913>",
            "pressureActive": "true",
            "root": "<4511.289551, 339.476898, 10243.327148>",
            "heading": "-165.386",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.278809, 340.456238, 10243.326172>",
            "previousHead": "<4511.198242, 341.114929, 10243.129883>",
            "previousLeftFoot": "<4511.397949, 339.899597, 10243.075195>",
            "previousRightFoot": "<4511.050293, 339.637878, 10243.146484>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "34.1998",
            "animUpdates": "684",
            "directorNow": "115.749",
            "directorBorn": "41.796",
            "engineTickTime": "115.755",
            "separationDistance": "2.25879"
          }
        },
        {
          "originalLanding": {
            "sequence": "203",
            "seconds": "79.093",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.474609, 339.412018, 10240.642578>",
            "tested": "<4511.496094, 339.182983, 10240.653320>",
            "supported": "<4511.496094, 339.182983, 10240.653320>",
            "heading": "-119.719",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "35.9997",
            "nativeAfter": "35.9997",
            "updatesBefore": "720",
            "updatesAfter": "720",
            "hipsMS": "<-0.010169, 0.979330, -0.003902>",
            "headMS": "<-0.038455, 1.638029, -0.213963>",
            "leftFootMS": "<0.168402, 0.422690, -0.216403>",
            "rightFootMS": "<-0.185973, 0.160976, -0.235643>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "203",
            "seconds": "79.093",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-5.35543",
            "pressureVelocity": "<0.599851, 0.000000, 0.299513>",
            "pressureActive": "true",
            "root": "<4511.474609, 339.412018, 10240.642578>",
            "heading": "-119.719",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.468262, 340.391479, 10240.646484>",
            "previousHead": "<4511.274414, 341.047546, 10240.547852>",
            "previousLeftFoot": "<4511.463379, 339.794708, 10240.454102>",
            "previousRightFoot": "<4511.083008, 339.635803, 10240.641602>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "35.9997",
            "animUpdates": "720",
            "directorNow": "120.889",
            "directorBorn": "41.796",
            "engineTickTime": "120.933",
            "separationDistance": "0.0615234"
          }
        },
        {
          "originalLanding": {
            "sequence": "225",
            "seconds": "83.237",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "gate": "WORLD:StaticObj_Container_1C",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.482422, 339.321655, 10243.132813>",
            "tested": "<4511.490234, 339.181000, 10243.136719>",
            "supported": "<4511.490234, 339.181000, 10243.136719>",
            "heading": "-114.719",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "97",
            "rejected": "97",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "37.2497",
            "nativeAfter": "37.2497",
            "updatesBefore": "745",
            "updatesAfter": "745",
            "hipsMS": "<-0.015595, 0.978175, 0.000270>",
            "headMS": "<-0.076743, 1.631960, -0.208661>",
            "leftFootMS": "<0.173523, 0.318747, -0.361671>",
            "rightFootMS": "<-0.136675, 0.181045, 0.058765>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "225",
            "seconds": "83.237",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.599851, 0.000000, 0.299513>",
            "pressureActive": "true",
            "root": "<4511.482422, 339.321655, 10243.132813>",
            "heading": "-114.719",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.476074, 340.299835, 10243.147461>",
            "previousHead": "<4511.260742, 340.953613, 10243.115234>",
            "previousLeftFoot": "<4511.226563, 339.640411, 10242.824219>",
            "previousRightFoot": "<4511.478516, 339.502686, 10243.281250>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "37.2497",
            "animUpdates": "745",
            "directorNow": "125.033",
            "directorBorn": "41.796",
            "engineTickTime": "125.041",
            "separationDistance": "2.54785"
          }
        },
        {
          "originalLanding": {
            "sequence": "94",
            "seconds": "54.519",
            "id": "1",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.262695, 340.213043, 10240.291992>",
            "tested": "<4511.273926, 340.181946, 10240.291016>",
            "supported": "<4511.273926, 340.181946, 10240.291016>",
            "heading": "-7.00159",
            "lift": "0.0557556",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "26.7499",
            "nativeAfter": "26.7499",
            "updatesBefore": "535",
            "updatesAfter": "535",
            "hipsMS": "<0.000000, 0.826965, 0.000000>",
            "headMS": "<-0.123693, 1.052246, -0.647012>",
            "leftFootMS": "<0.241849, 0.098283, -0.440106>",
            "rightFootMS": "<-0.261454, 0.093795, 0.287841>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "94",
            "seconds": "54.519",
            "id": "1",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-1.4905",
            "pressureVelocity": "<0.434810, 0.000000, -0.012211>",
            "pressureActive": "true",
            "root": "<4511.262695, 340.213043, 10240.291992>",
            "heading": "-7.00159",
            "lift": "0.0557556",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.332520, 341.096863, 10240.229492>",
            "previousHead": "<4511.389160, 341.319000, 10240.890625>",
            "previousLeftFoot": "<4511.040527, 340.367188, 10240.638672>",
            "previousRightFoot": "<4511.627441, 340.364380, 10239.977539>",
            "request": "6",
            "nextRequest": "6",
            "nativeRequest": "0",
            "nativeSeconds": "26.7499",
            "animUpdates": "535",
            "directorNow": "96.315",
            "directorBorn": "41.796",
            "engineTickTime": "96.329",
            "separationDistance": "0"
          }
        }
      ]
    },
    "owner": {
      "outcomes": {
        "VALIDATED_FOLLOW": 44
      },
      "helperFailures": {
        "LEASE_FAILED:TIME_BOUND": 2
      },
      "violations": []
    },
    "physicalOutcome": {
      "landingSpaceTrueSamples": 66,
      "physicalFallLandEvents": 60,
      "compatibleQuerySamples": 61,
      "uniquePhysicalEventsCovered": 60,
      "ambiguousQuerySamples": [
        {
          "landingSequence": 316,
          "id": 26,
          "generation": 1,
          "seconds": 112.865,
          "physicalMatches": [
            944
          ]
        },
        {
          "landingSequence": 317,
          "id": 26,
          "generation": 1,
          "seconds": 112.865,
          "physicalMatches": [
            944
          ]
        }
      ],
      "unmatchedQuerySamples": [
        {
          "landingSequence": 226,
          "id": 4,
          "generation": 1,
          "seconds": 83.522,
          "physicalMatches": []
        },
        {
          "landingSequence": 310,
          "id": 26,
          "generation": 1,
          "seconds": 112.683,
          "physicalMatches": []
        },
        {
          "landingSequence": 311,
          "id": 26,
          "generation": 1,
          "seconds": 112.683,
          "physicalMatches": []
        },
        {
          "landingSequence": 313,
          "id": 26,
          "generation": 1,
          "seconds": 112.763,
          "physicalMatches": []
        },
        {
          "landingSequence": 314,
          "id": 26,
          "generation": 1,
          "seconds": 112.763,
          "physicalMatches": []
        }
      ],
      "selectedUnmatchedEvidence": [
        {
          "query": {
            "landingSequence": 226,
            "id": 4,
            "generation": 1,
            "seconds": 83.522,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "226",
            "seconds": "83.522",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.482422, 339.321655, 10243.338867>",
            "tested": "<4511.490234, 339.180725, 10243.342773>",
            "supported": "<4511.490234, 339.180725, 10243.342773>",
            "heading": "-114.719",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "105",
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
          "previousContact": {
            "sequence": "226",
            "seconds": "83.522",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.599851, 0.000000, 0.299513>",
            "pressureActive": "true",
            "root": "<4511.482422, 339.321655, 10243.338867>",
            "heading": "-114.719",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.480957, 340.300262, 10243.343750>",
            "previousHead": "<4511.277832, 340.953308, 10243.251953>",
            "previousLeftFoot": "<4511.420410, 339.509338, 10243.132813>",
            "previousRightFoot": "<4511.306152, 339.682129, 10243.408203>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "37.3997",
            "animUpdates": "748",
            "directorNow": "125.318",
            "directorBorn": "41.796",
            "engineTickTime": "125.366",
            "separationDistance": "2.75391"
          },
          "matchingContactFailureRecords": [
            {
              "stage": "SWEEP",
              "joint": 0,
              "anchor": "<4511.490234, 339.180725, 10243.342773>",
              "previous": "<4511.480957, 340.300262, 10243.343750>",
              "raw": "<4511.488770, 340.159332, 10243.347656>",
              "target": "<4511.488770, 340.159332, 10243.347656>",
              "normal": "<0.890571, 0.332765, 0.310081>",
              "fraction": 0.306478,
              "proposedLift": 0.0,
              "collisionType": "OBJECT:StaticObj_Container_1C",
              "cachedPreviousDifferenceMetres": 0.0,
              "rawFromCapturedModelDifferenceMetres": 0.0004011593300806844,
              "absoluteDirectorSeconds": 125.318,
              "relativeLandingSeconds": 83.522,
              "directorBorn": 41.796,
              "clockOffsetResidualSeconds": -7.105427357601002e-15
            }
          ],
          "interpretation": "Matching rejection evidence"
        },
        {
          "query": {
            "landingSequence": 310,
            "id": 26,
            "generation": 1,
            "seconds": 112.683,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "310",
            "seconds": "112.683",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "tested": "<4514.628906, 339.227753, 10242.616211>",
            "supported": "<4514.628906, 339.227753, 10242.616211>",
            "heading": "-106.208",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "48.1496",
            "nativeAfter": "48.1496",
            "updatesBefore": "963",
            "updatesAfter": "963",
            "hipsMS": "<-0.003294, 0.979139, -0.001848>",
            "headMS": "<0.002646, 1.630308, -0.227402>",
            "leftFootMS": "<0.156295, 0.310295, -0.026279>",
            "rightFootMS": "<-0.185243, 0.299080, -0.368615>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "310",
            "seconds": "112.683",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.383945, 0.000000, -0.215104>",
            "pressureActive": "true",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "heading": "-106.208",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4514.620605, 340.403534, 10242.621094>",
            "previousHead": "<4514.242188, 340.935852, 10242.564453>",
            "previousLeftFoot": "<4514.815918, 339.754486, 10242.498047>",
            "previousRightFoot": "<4514.350098, 339.579498, 10242.690430>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "48.1496",
            "animUpdates": "963",
            "directorNow": "154.479",
            "directorBorn": "41.796",
            "engineTickTime": "154.486",
            "separationDistance": "1.46586"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        },
        {
          "query": {
            "landingSequence": 311,
            "id": 26,
            "generation": 1,
            "seconds": 112.683,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "311",
            "seconds": "112.683",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "tested": "<4514.627441, 339.227722, 10242.617188>",
            "supported": "<4514.627441, 339.227722, 10242.617188>",
            "heading": "-106.208",
            "lift": "0",
            "reason": "FALL_NATIVE_JOINT_WORLD_3",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "2",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "48.1496",
            "nativeAfter": "48.1496",
            "updatesBefore": "963",
            "updatesAfter": "963",
            "hipsMS": "<-0.003294, 0.979139, -0.001848>",
            "headMS": "<0.002646, 1.630308, -0.227402>",
            "leftFootMS": "<0.156295, 0.310295, -0.026279>",
            "rightFootMS": "<-0.185243, 0.299080, -0.368615>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "311",
            "seconds": "112.683",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.0249939",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.383945, 0.000000, -0.215104>",
            "pressureActive": "true",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "heading": "-106.208",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4514.620605, 340.403534, 10242.621094>",
            "previousHead": "<4514.242188, 340.935852, 10242.564453>",
            "previousLeftFoot": "<4514.815918, 339.754486, 10242.498047>",
            "previousRightFoot": "<4514.350098, 339.579498, 10242.690430>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "48.1496",
            "animUpdates": "963",
            "directorNow": "154.479",
            "directorBorn": "41.796",
            "engineTickTime": "154.49",
            "separationDistance": "1.46586"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        },
        {
          "query": {
            "landingSequence": 313,
            "id": 26,
            "generation": 1,
            "seconds": 112.763,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "313",
            "seconds": "112.763",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "tested": "<4514.628906, 339.227753, 10242.616211>",
            "supported": "<4514.628906, 339.227753, 10242.616211>",
            "heading": "-106.208",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "3",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "48.1996",
            "nativeAfter": "48.1996",
            "updatesBefore": "964",
            "updatesAfter": "964",
            "hipsMS": "<-0.006577, 0.979465, -0.003685>",
            "headMS": "<-0.017316, 1.635520, -0.220732>",
            "leftFootMS": "<0.158207, 0.382690, -0.103425>",
            "rightFootMS": "<-0.193561, 0.223782, -0.340688>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "313",
            "seconds": "112.763",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.383945, 0.000000, -0.215104>",
            "pressureActive": "true",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "heading": "-106.208",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4514.620605, 340.403534, 10242.621094>",
            "previousHead": "<4514.242188, 340.935852, 10242.564453>",
            "previousLeftFoot": "<4514.815918, 339.754486, 10242.498047>",
            "previousRightFoot": "<4514.350098, 339.579498, 10242.690430>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "48.1996",
            "animUpdates": "964",
            "directorNow": "154.559",
            "directorBorn": "41.796",
            "engineTickTime": "154.572",
            "separationDistance": "1.46586"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        },
        {
          "query": {
            "landingSequence": 314,
            "id": 26,
            "generation": 1,
            "seconds": 112.763,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "314",
            "seconds": "112.763",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "tested": "<4514.627441, 339.227722, 10242.617188>",
            "supported": "<4514.627441, 339.227722, 10242.617188>",
            "heading": "-106.208",
            "lift": "0",
            "reason": "FALL_NATIVE_JOINT_WORLD_3",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "4",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "48.1996",
            "nativeAfter": "48.1996",
            "updatesBefore": "964",
            "updatesAfter": "964",
            "hipsMS": "<-0.006577, 0.979465, -0.003685>",
            "headMS": "<-0.017316, 1.635520, -0.220732>",
            "leftFootMS": "<0.158207, 0.382690, -0.103425>",
            "rightFootMS": "<-0.193561, 0.223782, -0.340688>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "314",
            "seconds": "112.763",
            "id": "26",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.0269928",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.383945, 0.000000, -0.215104>",
            "pressureActive": "true",
            "root": "<4514.620605, 339.527344, 10242.621094>",
            "heading": "-106.208",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4514.620605, 340.403534, 10242.621094>",
            "previousHead": "<4514.242188, 340.935852, 10242.564453>",
            "previousLeftFoot": "<4514.815918, 339.754486, 10242.498047>",
            "previousRightFoot": "<4514.350098, 339.579498, 10242.690430>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "48.1996",
            "animUpdates": "964",
            "directorNow": "154.559",
            "directorBorn": "41.796",
            "engineTickTime": "154.576",
            "separationDistance": "1.46586"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        }
      ]
    },
    "callbackClock": {
      "observedMissionSeconds": 11.172,
      "nativeAdvanceSeconds": 7.6,
      "calls": 152,
      "nativeClockPerMissionSecond": 0.6802721088435374,
      "callbackCallsPerMissionSecond": 13.605442176870747,
      "nativeAdvancePerCallback": 0.049999999999999996
    },
    "clockExcluded": {
      "warmup": 103,
      "background": 2873
    }
  },
  {
    "runId": "runtime-20261029-193252",
    "count": 64,
    "state": {
      "terminalArrivals": 64,
      "highMisses": [
        0,
        2,
        3,
        4,
        8,
        11,
        14,
        16,
        19,
        23,
        24,
        29,
        31,
        32,
        34,
        36,
        41,
        43,
        44,
        47,
        48,
        52,
        54,
        55,
        57,
        58,
        59
      ],
      "fallsResumed": 55,
      "labels": {
        "CONTACT_SUPPORT_LOST": 19
      },
      "frameMs": {
        "samples": 4626,
        "p50": 52.0,
        "p95": 149.0,
        "p99": 195.0
      },
      "allPhysicalTransitionsMatched": true
    },
    "cadenceAppActive": {
      "true": 4685
    },
    "actualPlayerFirstLastDisplacementMetres": {
      "GROUND_EAST": 38.7066742951589,
      "GROUND_NORTH": 14.635898244626505
    },
    "previousContact": {
      "parentRows": 106,
      "joinedRows": 106,
      "dt": {
        "min": 0.00299072,
        "p50": 0.05,
        "max": 0.05
      },
      "hist": {
        "0": 0,
        "4": 106
      },
      "selectedSnapshots": [
        {
          "originalLanding": {
            "sequence": "157",
            "seconds": "56.137",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "gate": "FIXTURE_VOLUME",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.427246, 339.529144, 10240.584961>",
            "tested": "<4511.451172, 339.181946, 10240.578125>",
            "supported": "<4511.451172, 339.181976, 10240.578125>",
            "heading": "-137.013",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "29.7998",
            "nativeAfter": "29.7998",
            "updatesBefore": "596",
            "updatesAfter": "596",
            "hipsMS": "<-0.010169, 0.979330, -0.003902>",
            "headMS": "<-0.038455, 1.638029, -0.213963>",
            "leftFootMS": "<0.168402, 0.422690, -0.216403>",
            "rightFootMS": "<-0.185973, 0.160976, -0.235643>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "157",
            "seconds": "56.137",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-5.40453",
            "pressureVelocity": "<0.499243, 0.000000, -0.145222>",
            "pressureActive": "true",
            "root": "<4511.427246, 339.529144, 10240.584961>",
            "heading": "-137.013",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.419922, 340.508606, 10240.586914>",
            "previousHead": "<4511.264160, 341.164673, 10240.435547>",
            "previousLeftFoot": "<4511.472656, 339.911835, 10240.401367>",
            "previousRightFoot": "<4511.053223, 339.752930, 10240.467773>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "29.7998",
            "animUpdates": "596",
            "directorNow": "100.51",
            "directorBorn": "44.373",
            "engineTickTime": "100.517",
            "separationDistance": "0.0517578"
          }
        },
        {
          "originalLanding": {
            "sequence": "195",
            "seconds": "60.703",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "gate": "WORLD:StaticObj_Container_1C",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.441406, 339.370117, 10243.199219>",
            "tested": "<4511.449219, 339.180328, 10243.197266>",
            "supported": "<4511.449219, 339.180328, 10243.197266>",
            "heading": "-140.013",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "112",
            "rejected": "112",
            "budgetSkipped": "1",
            "poseAvailable": "true",
            "nativeBefore": "32.0998",
            "nativeAfter": "32.0998",
            "updatesBefore": "642",
            "updatesAfter": "642",
            "hipsMS": "<-0.015857, 0.978502, -0.001683>",
            "headMS": "<-0.078992, 1.633739, -0.207273>",
            "leftFootMS": "<0.177774, 0.383290, -0.361676>",
            "rightFootMS": "<-0.152805, 0.143002, -0.003748>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "195",
            "seconds": "60.703",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.499243, 0.000000, -0.145222>",
            "pressureActive": "true",
            "root": "<4511.441406, 339.370117, 10243.199219>",
            "heading": "-140.013",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.428223, 340.348633, 10243.208008>",
            "previousHead": "<4511.247559, 341.003845, 10243.090820>",
            "previousLeftFoot": "<4511.345215, 339.753418, 10242.807617>",
            "previousRightFoot": "<4511.321777, 339.513123, 10243.294922>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "32.0998",
            "animUpdates": "642",
            "directorNow": "105.076",
            "directorBorn": "44.373",
            "engineTickTime": "105.084",
            "separationDistance": "2.66992"
          }
        },
        {
          "originalLanding": {
            "sequence": "103",
            "seconds": "51.551",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.254395, 340.211884, 10238.861328>",
            "tested": "<4511.265137, 340.177948, 10238.862305>",
            "supported": "<4511.265137, 340.177979, 10238.862305>",
            "heading": "-30.685",
            "lift": "0.0200806",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "27.6499",
            "nativeAfter": "27.6499",
            "updatesBefore": "553",
            "updatesAfter": "553",
            "hipsMS": "<0.000000, 0.827597, 0.000000>",
            "headMS": "<-0.024057, 1.157499, -0.615202>",
            "leftFootMS": "<0.230665, 0.096668, -0.414108>",
            "rightFootMS": "<-0.272178, 0.094106, 0.314204>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "103",
            "seconds": "51.551",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-1.4905",
            "pressureVelocity": "<0.408767, 0.000000, 0.016354>",
            "pressureActive": "true",
            "root": "<4511.254395, 340.211884, 10238.861328>",
            "heading": "-30.685",
            "lift": "0.0200806",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.254395, 341.062378, 10238.861328>",
            "previousHead": "<4510.943359, 341.381683, 10239.400391>",
            "previousLeftFoot": "<4510.832031, 340.327881, 10239.094727>",
            "previousRightFoot": "<4511.636230, 340.323639, 10238.726563>",
            "request": "6",
            "nextRequest": "6",
            "nativeRequest": "0",
            "nativeSeconds": "27.6499",
            "animUpdates": "553",
            "directorNow": "95.924",
            "directorBorn": "44.373",
            "engineTickTime": "95.957",
            "separationDistance": "0"
          }
        },
        {
          "originalLanding": {
            "sequence": "149",
            "seconds": "55.341",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "gate": "WORLD:StaticObj_Container_1C",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.291016, 339.454620, 10236.642578>",
            "tested": "<4511.296387, 339.167450, 10236.642578>",
            "supported": "<4511.296387, 339.167450, 10236.642578>",
            "heading": "-27.685",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "58",
            "rejected": "58",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "29.4998",
            "nativeAfter": "29.4998",
            "updatesBefore": "590",
            "updatesAfter": "590",
            "hipsMS": "<0.000000, 0.876198, 0.000000>",
            "headMS": "<-0.050839, 1.408517, -0.379067>",
            "leftFootMS": "<0.172712, 0.227151, 0.153251>",
            "rightFootMS": "<-0.142326, 0.052159, -0.240494>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "149",
            "seconds": "55.341",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.0129929",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.408767, 0.000000, 0.016354>",
            "pressureActive": "true",
            "root": "<4511.291016, 339.454620, 10236.642578>",
            "heading": "-27.685",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.291016, 340.330811, 10236.642578>",
            "previousHead": "<4511.160156, 340.863129, 10237.001953>",
            "previousLeftFoot": "<4511.209473, 339.681763, 10236.426758>",
            "previousRightFoot": "<4511.305176, 339.506775, 10236.921875>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "29.4998",
            "animUpdates": "590",
            "directorNow": "99.714",
            "directorBorn": "44.373",
            "engineTickTime": "99.758",
            "separationDistance": "2.21973"
          }
        },
        {
          "originalLanding": {
            "sequence": "127",
            "seconds": "53.584",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "gate": "FOOT_X_SUPPORT",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.260254, 340.219269, 10241.096680>",
            "tested": "<4511.279297, 340.184204, 10241.098633>",
            "supported": "<4511.279297, 340.184204, 10241.098633>",
            "heading": "166.743",
            "lift": "0.0207825",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "1",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "28.6498",
            "nativeAfter": "28.6498",
            "updatesBefore": "573",
            "updatesAfter": "573",
            "hipsMS": "<0.000000, 0.827578, 0.000000>",
            "headMS": "<-0.102010, 1.154734, -0.596173>",
            "leftFootMS": "<0.214305, 0.097967, -0.393462>",
            "rightFootMS": "<-0.289228, 0.095684, 0.338271>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "127",
            "seconds": "53.584",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-1.4905",
            "pressureVelocity": "<0.728063, 0.000000, 0.070496>",
            "pressureActive": "true",
            "root": "<4511.260254, 340.219269, 10241.096680>",
            "heading": "166.743",
            "lift": "0.0207825",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.227051, 341.075165, 10241.113281>",
            "previousHead": "<4511.241699, 341.374146, 10240.486328>",
            "previousLeftFoot": "<4511.520020, 340.337250, 10240.775391>",
            "previousRightFoot": "<4510.862793, 340.334503, 10241.370117>",
            "request": "6",
            "nextRequest": "6",
            "nativeRequest": "0",
            "nativeSeconds": "28.6498",
            "animUpdates": "573",
            "directorNow": "97.957",
            "directorBorn": "44.373",
            "engineTickTime": "97.961",
            "separationDistance": "0"
          }
        },
        {
          "originalLanding": {
            "sequence": "163",
            "seconds": "57.373",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "gate": "WORLD:StaticObj_Container_1C",
            "valid": "false",
            "requireEntitySpace": "false",
            "root": "<4511.298828, 339.464722, 10243.380859>",
            "tested": "<4511.308594, 339.178040, 10243.381836>",
            "supported": "<4511.308594, 339.178040, 10243.381836>",
            "heading": "166.743",
            "lift": "0",
            "reason": "NATIVE_FALL",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "92",
            "rejected": "92",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "30.4998",
            "nativeAfter": "30.4998",
            "updatesBefore": "610",
            "updatesAfter": "610",
            "hipsMS": "<-0.003294, 0.979139, -0.001848>",
            "headMS": "<0.002646, 1.630308, -0.227402>",
            "leftFootMS": "<0.156295, 0.310295, -0.026279>",
            "rightFootMS": "<-0.185243, 0.299080, -0.368615>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "163",
            "seconds": "57.373",
            "id": "4",
            "generation": "1",
            "phase": "FALL",
            "valid": "false",
            "insideFall": "true",
            "fallDt": "0.0149918",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.728063, 0.000000, 0.070496>",
            "pressureActive": "true",
            "root": "<4511.298828, 339.464722, 10243.380859>",
            "heading": "166.743",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.295898, 340.443848, 10243.377930>",
            "previousHead": "<4511.353516, 341.095032, 10243.160156>",
            "previousLeftFoot": "<4511.457031, 339.775024, 10243.391602>",
            "previousRightFoot": "<4511.203125, 339.763794, 10242.979492>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "30.4998",
            "animUpdates": "610",
            "directorNow": "101.746",
            "directorBorn": "44.373",
            "engineTickTime": "101.749",
            "separationDistance": "2.28027"
          }
        }
      ]
    },
    "owner": {
      "outcomes": {
        "VALIDATED_FOLLOW": 40
      },
      "helperFailures": {
        "LEASE_FAILED:DISTANCE_BOUND": 2,
        "LEASE_FAILED:TIME_BOUND": 1
      },
      "violations": []
    },
    "physicalOutcome": {
      "landingSpaceTrueSamples": 60,
      "physicalFallLandEvents": 55,
      "compatibleQuerySamples": 55,
      "uniquePhysicalEventsCovered": 55,
      "ambiguousQuerySamples": [],
      "unmatchedQuerySamples": [
        {
          "landingSequence": 152,
          "id": 32,
          "generation": 1,
          "seconds": 55.602,
          "physicalMatches": []
        },
        {
          "landingSequence": 197,
          "id": 23,
          "generation": 1,
          "seconds": 61.128,
          "physicalMatches": []
        },
        {
          "landingSequence": 232,
          "id": 34,
          "generation": 1,
          "seconds": 70.169,
          "physicalMatches": []
        },
        {
          "landingSequence": 233,
          "id": 34,
          "generation": 1,
          "seconds": 70.169,
          "physicalMatches": []
        },
        {
          "landingSequence": 284,
          "id": 43,
          "generation": 1,
          "seconds": 81.208,
          "physicalMatches": []
        }
      ],
      "selectedUnmatchedEvidence": [
        {
          "query": {
            "landingSequence": 152,
            "id": 32,
            "generation": 1,
            "seconds": 55.602,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "152",
            "seconds": "55.602",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.291016, 339.454620, 10236.513672>",
            "tested": "<4511.298340, 339.166962, 10236.513672>",
            "supported": "<4511.298340, 339.166962, 10236.513672>",
            "heading": "-27.685",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "63",
            "rejected": "62",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "29.5498",
            "nativeAfter": "29.5498",
            "updatesBefore": "591",
            "updatesAfter": "591",
            "hipsMS": "<-0.003294, 0.979139, -0.001848>",
            "headMS": "<0.002646, 1.630308, -0.227402>",
            "leftFootMS": "<0.156295, 0.310295, -0.026279>",
            "rightFootMS": "<-0.185243, 0.299080, -0.368615>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "152",
            "seconds": "55.602",
            "id": "32",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.0279846",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.408767, 0.000000, 0.016354>",
            "pressureActive": "true",
            "root": "<4511.291016, 339.454620, 10236.513672>",
            "heading": "-27.685",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.292969, 340.433746, 10236.516602>",
            "previousHead": "<4511.183105, 341.084930, 10236.713867>",
            "previousLeftFoot": "<4511.140625, 339.764923, 10236.463867>",
            "previousRightFoot": "<4511.283691, 339.753693, 10236.925781>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "29.5498",
            "animUpdates": "591",
            "directorNow": "99.975",
            "directorBorn": "44.373",
            "engineTickTime": "99.991",
            "separationDistance": "2.34863"
          },
          "matchingContactFailureRecords": [
            {
              "stage": "SWEEP",
              "joint": 0,
              "anchor": "<4511.298340, 339.166962, 10236.513672>",
              "previous": "<4511.292969, 340.433746, 10236.516602>",
              "raw": "<4511.300293, 340.146088, 10236.516602>",
              "target": "<4511.300293, 340.146088, 10236.516602>",
              "normal": "<-0.004777, 0.255935, -0.966682>",
              "fraction": 0.728743,
              "proposedLift": 0.0,
              "collisionType": "OBJECT:StaticObj_Container_1C",
              "cachedPreviousDifferenceMetres": 0.0,
              "rawFromCapturedModelDifferenceMetres": 0.00025953107283592893,
              "absoluteDirectorSeconds": 99.975,
              "relativeLandingSeconds": 55.602,
              "directorBorn": 44.373,
              "clockOffsetResidualSeconds": 0.0
            }
          ],
          "interpretation": "Matching rejection evidence"
        },
        {
          "query": {
            "landingSequence": 197,
            "id": 23,
            "generation": 1,
            "seconds": 61.128,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "197",
            "seconds": "61.128",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.441406, 339.370117, 10243.416016>",
            "tested": "<4511.449219, 339.180054, 10243.414063>",
            "supported": "<4511.449219, 339.180023, 10243.414063>",
            "heading": "-140.013",
            "lift": "0",
            "reason": "LANDING_SPACE_BLOCKED",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "120",
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
          "previousContact": {
            "sequence": "197",
            "seconds": "61.128",
            "id": "23",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.499243, 0.000000, -0.145222>",
            "pressureActive": "true",
            "root": "<4511.441406, 339.370117, 10243.416016>",
            "heading": "-140.013",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.435547, 340.348511, 10243.422852>",
            "previousHead": "<4511.279785, 341.003967, 10243.270508>",
            "previousLeftFoot": "<4511.410156, 339.575531, 10243.125000>",
            "previousRightFoot": "<4511.337891, 339.679504, 10243.487305>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "32.2498",
            "animUpdates": "645",
            "directorNow": "105.501",
            "directorBorn": "44.373",
            "engineTickTime": "105.51",
            "separationDistance": "2.88672"
          },
          "matchingContactFailureRecords": [
            {
              "stage": "SWEEP",
              "joint": 0,
              "anchor": "<4511.449219, 339.180023, 10243.414063>",
              "previous": "<4511.435547, 340.348511, 10243.422852>",
              "raw": "<4511.443359, 340.158417, 10243.420898>",
              "target": "<4511.443359, 340.158417, 10243.420898>",
              "normal": "<0.673966, 0.247264, 0.696154>",
              "fraction": 0.571142,
              "proposedLift": 0.0,
              "collisionType": "OBJECT:StaticObj_Container_1C",
              "cachedPreviousDifferenceMetres": 0.0,
              "rawFromCapturedModelDifferenceMetres": 0.0002778333440593949,
              "absoluteDirectorSeconds": 105.501,
              "relativeLandingSeconds": 61.128,
              "directorBorn": 44.373,
              "clockOffsetResidualSeconds": 7.105427357601002e-15
            }
          ],
          "interpretation": "Matching rejection evidence"
        },
        {
          "query": {
            "landingSequence": 232,
            "id": 34,
            "generation": 1,
            "seconds": 70.169,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "232",
            "seconds": "70.169",
            "id": "34",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.513184, 339.307953, 10240.511719>",
            "tested": "<4511.530762, 339.183258, 10240.508789>",
            "supported": "<4511.530762, 339.183258, 10240.508789>",
            "heading": "-171.148",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "1",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "36.7997",
            "nativeAfter": "36.7997",
            "updatesBefore": "736",
            "updatesAfter": "736",
            "hipsMS": "<-0.013916, 0.978965, -0.003309>",
            "headMS": "<-0.060617, 1.637057, -0.209091>",
            "leftFootMS": "<0.175260, 0.422555, -0.312512>",
            "rightFootMS": "<-0.171680, 0.134666, -0.111536>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "232",
            "seconds": "70.169",
            "id": "34",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-4.76692",
            "pressureVelocity": "<0.652024, 0.000000, -0.125814>",
            "pressureActive": "true",
            "root": "<4511.513184, 339.307953, 10240.511719>",
            "heading": "-171.148",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.502441, 340.287292, 10240.509766>",
            "previousHead": "<4511.442383, 340.945984, 10240.306641>",
            "previousLeftFoot": "<4511.646484, 339.730652, 10240.272461>",
            "previousRightFoot": "<4511.292969, 339.468933, 10240.307617>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "36.7997",
            "animUpdates": "736",
            "directorNow": "114.542",
            "directorBorn": "44.373",
            "engineTickTime": "114.584",
            "separationDistance": "0.0214844"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        },
        {
          "query": {
            "landingSequence": 233,
            "id": 34,
            "generation": 1,
            "seconds": 70.169,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "233",
            "seconds": "70.169",
            "id": "34",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.513184, 339.307953, 10240.511719>",
            "tested": "<4511.529297, 339.183228, 10240.508789>",
            "supported": "<4511.529297, 339.183258, 10240.508789>",
            "heading": "-171.148",
            "lift": "0",
            "reason": "FALL_NATIVE_JOINT_WORLD_2",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "2",
            "rejected": "0",
            "budgetSkipped": "0",
            "poseAvailable": "true",
            "nativeBefore": "36.7997",
            "nativeAfter": "36.7997",
            "updatesBefore": "736",
            "updatesAfter": "736",
            "hipsMS": "<-0.013916, 0.978965, -0.003309>",
            "headMS": "<-0.060617, 1.637057, -0.209091>",
            "leftFootMS": "<0.175260, 0.422555, -0.312512>",
            "rightFootMS": "<-0.171680, 0.134666, -0.111536>",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "link": "none"
          },
          "previousContact": {
            "sequence": "233",
            "seconds": "70.169",
            "id": "34",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.0479965",
            "fallVelocity": "-5.23776",
            "pressureVelocity": "<0.652024, 0.000000, -0.125814>",
            "pressureActive": "true",
            "root": "<4511.513184, 339.307953, 10240.511719>",
            "heading": "-171.148",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.502441, 340.287292, 10240.509766>",
            "previousHead": "<4511.442383, 340.945984, 10240.306641>",
            "previousLeftFoot": "<4511.646484, 339.730652, 10240.272461>",
            "previousRightFoot": "<4511.292969, 339.468933, 10240.307617>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "36.7997",
            "animUpdates": "736",
            "directorNow": "114.542",
            "directorBorn": "44.373",
            "engineTickTime": "114.587",
            "separationDistance": "0.0214844"
          },
          "matchingContactFailureRecords": [],
          "interpretation": "No matched failure detail;cause unproven,not absence of collision"
        },
        {
          "query": {
            "landingSequence": 284,
            "id": 43,
            "generation": 1,
            "seconds": 81.208,
            "physicalMatches": []
          },
          "originalLanding": {
            "sequence": "284",
            "seconds": "81.208",
            "id": "43",
            "generation": "1",
            "phase": "FALL",
            "gate": "CLEAR",
            "valid": "true",
            "requireEntitySpace": "false",
            "root": "<4511.488281, 339.377655, 10236.717773>",
            "tested": "<4511.496094, 339.171448, 10236.717773>",
            "supported": "<4511.496094, 339.171478, 10236.717773>",
            "heading": "-105.053",
            "lift": "0",
            "reason": "FRAME_FALL_PENDING",
            "failure": "CONTACT_SUPPORT_LOST",
            "checks": "117",
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
          "previousContact": {
            "sequence": "284",
            "seconds": "81.208",
            "id": "43",
            "generation": "1",
            "phase": "FALL",
            "valid": "true",
            "insideFall": "true",
            "fallDt": "0.05",
            "fallVelocity": "-15",
            "pressureVelocity": "<0.459561, 0.000000, -0.029091>",
            "pressureActive": "true",
            "root": "<4511.488281, 339.377655, 10236.717773>",
            "heading": "-105.053",
            "lift": "0",
            "idleOffset": "<0.000000, 0.000000, 0.000000>",
            "contactCount": "4",
            "previousHips": "<4511.487793, 340.356262, 10236.722656>",
            "previousHead": "<4511.271973, 341.009308, 10236.666016>",
            "previousLeftFoot": "<4511.392578, 339.565338, 10236.525391>",
            "previousRightFoot": "<4511.326172, 339.738129, 10236.816406>",
            "request": "8",
            "nextRequest": "8",
            "nativeRequest": "8",
            "nativeSeconds": "41.8496",
            "animUpdates": "837",
            "directorNow": "125.581",
            "directorBorn": "44.373",
            "engineTickTime": "125.587",
            "separationDistance": "3.23865"
          },
          "matchingContactFailureRecords": [
            {
              "stage": "SWEEP",
              "joint": 0,
              "anchor": "<4511.496094, 339.171478, 10236.717773>",
              "previous": "<4511.487793, 340.356262, 10236.722656>",
              "raw": "<4511.496094, 340.150269, 10236.718750>",
              "target": "<4511.496094, 340.150269, 10236.718750>",
              "normal": "<0.933897, 0.357541, -0.000909>",
              "fraction": 0.542663,
              "proposedLift": 0.0,
              "collisionType": "OBJECT:StaticObj_Container_1C",
              "cachedPreviousDifferenceMetres": 0.0,
              "rawFromCapturedModelDifferenceMetres": 0.0003628011745964658,
              "absoluteDirectorSeconds": 125.581,
              "relativeLandingSeconds": 81.208,
              "directorBorn": 44.373,
              "clockOffsetResidualSeconds": 7.105427357601002e-15
            }
          ],
          "interpretation": "Matching rejection evidence"
        }
      ]
    },
    "callbackClock": {
      "observedMissionSeconds": 294.9040000000015,
      "nativeAdvanceSeconds": 175.606,
      "calls": 3512,
      "nativeClockPerMissionSecond": 0.5954683558039197,
      "callbackCallsPerMissionSecond": 11.90896020399853,
      "nativeAdvancePerCallback": 0.050001708428246014
    },
    "clockExcluded": {
      "warmup": 59
    }
  }
]

Read-only observer module:
// Read-only companion to the already sampled LDObserveLanding row.
// No new physics/bone query, motion proposal or acceptance decision.
modded class DGBNativeTraversal
{
    bool PCRead;bool PCEnabled;bool PCInsideFall;float PCFallDt=-1;
    bool PCOn()
    {
        if(!PCRead){PCEnabled=FileExist("$profile:previous-contact-detail.flag");PCRead=true;}
        return PCEnabled;
    }
    override void Fall(float dt)
    {
        if(!PCOn()){super.Fall(dt);return;}
        bool before=PCInsideFall;float oldDt=PCFallDt;PCInsideFall=true;PCFallDt=dt;
        super.Fall(dt);
        PCInsideFall=before;PCFallDt=oldDt;
    }
    override void LDObserveLanding(vector tested,vector supported,bool valid,bool requireEntitySpace)
    {
        if(!PCOn()){super.LDObserveLanding(tested,supported,valid,requireEntitySpace);return;}
        int before=LDSequence;
        super.LDObserveLanding(tested,supported,valid,requireEntitySpace);
        // Reuse existing failure throttle and per-frame budget. Backoff rows do
        // not call this method, so this sequence joins one landing row exactly.
        if(LDSequence==before)return;
        string path="$profile:native-previous-contacts.tsv";bool exists=FileExist(path);
        FileHandle file=OpenFile(path,FileMode.APPEND);
        if(!file){Print("DG_PREVIOUS_CONTACT_LOG_FAILED");return;}
        if(!exists)FPrintln(file,"sequence\tseconds\tid\tgeneration\tphase\tvalid\tinsideFall\tfallDt\tfallVelocity\tpressureVelocity\tpressureActive\troot\theading\tlift\tidleOffset\tcontactCount\tpreviousHips\tpreviousHead\tpreviousLeftFoot\tpreviousRightFoot\trequest\tnextRequest\tnativeRequest\tnativeSeconds\tanimUpdates\tdirectorNow\tdirectorBorn\tengineTickTime\tseparationDistance");
        int count=Body.Contacts.Count();
        string hips="UNAVAILABLE";string head="UNAVAILABLE";string left="UNAVAILABLE";string right="UNAVAILABLE";
        if(count==4)
        {hips=Body.Contacts[0].ToString();head=Body.Contacts[1].ToString();left=Body.Contacts[2].ToString();right=Body.Contacts[3].ToString();}
        float dt=-1;if(PCInsideFall)dt=PCFallDt;
        string row=LDSequence.ToString()+"\t"+(Now()-Body.Director.Born)+"\t"+Body.Id+"\t"+CTRecoveryGeneration+"\t"+Phase;
        row+="\t"+valid+"\t"+PCInsideFall+"\t"+dt+"\t"+FallVelocity+"\t"+PressureFallVelocity+"\t"+PressureFallActive;
        row+="\t"+Body.Agent.Position+"\t"+Body.Agent.Heading+"\t"+Body.Lift+"\t"+Body.IdleOffset+"\t"+count;
        row+="\t"+hips+"\t"+head+"\t"+left+"\t"+right+"\t"+Body.Request+"\t"+Body.NextRequest;
        row+="\t"+Body.Leader.NativeRequest+"\t"+Body.Leader.NativeSeconds+"\t"+Body.Leader.AnimUpdates;
        row+="\t"+Body.Director.Now+"\t"+Body.Director.Born+"\t"+GetGame().GetTickTime()+"\t"+SeparationEpisodeDistance;
        FPrintln(file,row);CloseFile(file);
    }
}

Outcome join function:
"""Distinguish sampled support permission from actual physical LAND events."""
import argparse
import collections
import csv
import hashlib
import json
import math
import re
from pathlib import Path

def vec(text):return tuple(float(v) for v in re.findall(r'-?\d+(?:\.\d+)?(?:[eE][+-]?\d+)?',text))
def distance(a,b):return math.dist(vec(a),vec(b))

def analyze(profile):
    hashes={}
    def rows(name):
        path=profile/name;hashes[name]=hashlib.sha256(path.read_bytes()).hexdigest()
        with path.open(encoding='utf-8-sig') as f:return list(csv.DictReader(f,delimiter='\t'))
    landing=rows('native-landing-detail.tsv');previous=rows('native-previous-contacts.tsv')
    events=rows('native-state-contract.tsv');failures=rows('native-contact-failures.tsv')
    by={r['sequence']:r for r in previous}
    actual=[r for r in events if r['kind']=='TRANSITION' and r['previous']=='FALL' and r['next']=='LAND']
    joins=[]
    for r in landing:
        if r['valid']!='true':continue
        matches=[t for t in actual if t['id']==r['id'] and t['recoveryGeneration']==r['generation']
            and abs(float(t['seconds'])-float(r['seconds']))<.005 and distance(t['position'],r['supported'])<.000002]
        assert len(matches)<=1
        joins.append(dict(landingSequence=int(r['sequence']),id=int(r['id']),generation=int(r['generation']),
            seconds=float(r['seconds']),physicalMatches=[int(t['sequence']) for t in matches]))
    inverse=collections.Counter(e for r in joins for e in r['physicalMatches'])
    unmatched=[r for r in joins if not r['physicalMatches']]
    ambiguous=[r for r in joins if any(inverse[e]>1 for e in r['physicalMatches'])]
    causes=[]
    raw={int(r['sequence']):r for r in landing}
    for r in unmatched:
        source=raw[r['landingSequence']];snapshot=by[str(r['landingSequence'])]
        matches=[f for f in failures if f['id']==source['id']
            and abs(float(f['seconds'])-float(snapshot['directorNow']))<.005
            and abs(float(f['nativeSeconds'])-float(snapshot['nativeSeconds']))<.0001
            and distance(f['anchor'],source['supported'])<.000002]
        projected=[]
        for f in matches:
            joint=int(f['joint']);cached=['previousHips','previousHead','previousLeftFoot','previousRightFoot'][joint]
            model=['hipsMS','headMS','leftFootMS','rightFootMS'][joint]
            root=vec(source['supported']);ms=vec(source[model]);idle=vec(source['idleOffset']);angle=float(source['heading'])*math.pi/180
            x,y,z=[ms[i]+idle[i] for i in range(3)]
            predicted=(root[0]-math.cos(angle)*x-math.sin(angle)*z,root[1]+y,root[2]+math.sin(angle)*x-math.cos(angle)*z)
            projected.append(dict(stage=f['stage'],joint=joint,anchor=f['anchor'],previous=f['previous'],raw=f['raw'],target=f['target'],
                normal=f['hitNormal'],fraction=float(f['hitFraction']),proposedLift=float(f['proposedLift']),
                collisionType=f['collisionType'].split('|',1)[0],
                cachedPreviousDifferenceMetres=distance(f['previous'],snapshot[cached]),
                rawFromCapturedModelDifferenceMetres=math.dist(predicted,vec(f['raw'])),
                absoluteDirectorSeconds=float(f['seconds']),relativeLandingSeconds=float(source['seconds']),
                directorBorn=float(snapshot['directorBorn']),clockOffsetResidualSeconds=float(f['seconds'])-float(source['seconds'])-float(snapshot['directorBorn'])))
        causes.append(dict(query=r,originalLanding=source,previousContact=snapshot,matchingContactFailureRecords=projected,
            interpretation='Matching rejection evidence' if projected else 'No matched failure detail;cause unproven,not absence of collision'))
    return dict(schemaVersion=1,sourceRunId=profile.name,landingSpaceTrueSamples=len(joins),physicalFallLandEvents=len(actual),
        compatibleQuerySamples=sum(bool(r['physicalMatches']) for r in joins),uniquePhysicalEventsCovered=len(inverse),
        ambiguousQuerySamples=ambiguous,unmatchedQuerySamples=unmatched,selectedUnmatchedEvidence=causes,
        scope='Same id/generation,director time<5ms,support position<2micrometres. Repeated identical queries can share one event;no per-Fall-call ID is captured.',
        contactClockRule='Contact-failure seconds is absolute Director.Now;landing seconds subtractsDirector.Born. Join uses captureddirectorNow,never rawrelative seconds alone.',
        limitations=['A LandingSpace true row is permission for a proposed footprint,not automatically a physical LAND.',
            'No exact per-Fall-call result or serial;ambiguous repeated queries remain ambiguous.',
            'Failure details are throttled;no matching detail doesnot prove no world/peer rejection.',
            'Model-to-world reconstruction is double-precision posthoc,not native float bit identity.'],inputHashes=hashes)

def main():
    parser=argparse.ArgumentParser();parser.add_argument('profile',type=Path);parser.add_argument('--output',type=Path,required=True);a=parser.parse_args()
    assert not a.output.exists();d=analyze(a.profile)
    with a.output.open('x',encoding='utf-8') as f:json.dump(d,f,ensure_ascii=False,indent=2)
    print(json.dumps({k:v for k,v in d.items() if k not in ['selectedUnmatchedEvidence','inputHashes','limitations']}))

if __name__=='__main__':main()

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
