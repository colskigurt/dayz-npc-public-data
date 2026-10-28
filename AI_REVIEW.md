# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261028T195520-clock-conservation-repeat-20261029

활성 창400표본에서 시간 회계 재현 — 수정 경로 한계와43번 출구 실패 분리

작성: 2026-10-29T04:54:41.007275+09:00

1. 작업 목표

입력 dt·누적기 초기화·graph-owner 시계를 같은 호출에서 읽은 결과가 반복되는지 확인한다. 활성 창 상태가 유지된 표본을 확보하고 실제 영상 대응을 추가한다. 기존 B8FB AI와 정상 낙하·복구는 보존하며, 평균속도·시각 품질·64명 기능·성능을 별도로 판정한다.

2. 실제 수행

첫043225 실행을 보존하고 새044620 실행을120초 진행했다. 동일3개 서명PBO·고정run·AI/이동OFF·1논리개체/8부품·VSyncON 조건이다. 로딩 중 창을 전경으로 가져왔고, 이번 observer는 실제 HUD의 appActive=true가5초 유지된 뒤 attach했다. 기본 watcher는0초로 유지하며 이번만5초를 지정했다. helper 진입 한 곳에서 입력 spill·누적기·retained duration·graph clock을 읽었고 숫자/인자/상태 쓰기나 반환 hook은 없다. 게임과45초 녹화는 정상 종료됐다.

분석기의 callback 연결은 float값을 다시 검색하는 대신 실제 event ordinal로 연결하도록 보완했다. 입력/누적기/graph 산술과 원점/파일ACK/HUD의 검사는 유지했다. 새 원시·기존 보고서는 덮어쓰지 않았다.

처음 영상 분석의 두3초 구간에서는 native/CPU/영상 교집합이103쌍이었다. 이를 전체 계측 구간의 화면 증거로 확대하지 않았다. 기존 정확HUD의 mission-video 시간차로 native 첫 활성 연결0.5초 후부터6초를 선정해 video25.413/28.413초에서 다시 추출했다. 새파일은conservation-window-*로 저장했다.

3. 측정 결과

|항목|첫 실행043225|반복044620|
|---|---:|---:|
|호출|368|400|
|연속쌍/정확 graph 증가|367|399|
|활성 창 호출|364|400|
|입력 합 ms|7964.000149|7969.000174|
|graph 증가 ms|5249.919891|5149.921417|
|버린 초과분 ms|2747.000050|2853.000008|
|회계 잔차 ms|0.000000|0.000000|
|graph/진입 벽시간|0.658766|0.646232|

두 실행 합768호출·766연속쌍에서 graph 증가가 해당float32 식과 일치했다. 양쪽 누적기 예측잔차와 명시적float32 반올림 후 회계잔차는0이다. 첫 실행은 background4개를 분리한 관측이며, 새 실행은400개 모두 활성 창의 고유HUD에 연결됐다. 서로 다른 포커스 조건을 같은 조건의 완전한 반복으로 숨기지 않는다. 반복된 것은 시간 전달 구조다.

새 실행 입력/native 진입시간 비율0.999980, graph/입력0.646244.103초과 중 초과분 중앙약30ms·최대33ms였다. script callback 증가5149.900ms와graph5149.921417ms의 차이는−0.021417ms다. callback 문자열 반올림을 고려하며 비트일치를 주장하지 않는다.

새 실행399 CPU/source proxy 쌍의 적합p95=11.698mm, 시간잔차RMS=2.834ms, 상관0.977028. 처음분석 영상103쌍 상관0.941203은 보존했고, 추가로 선정한6초는360개HUD 모두정확·298 native/CPU/영상쌍·CPU/몸mask변화 상관0.948443이었다. 지각 품질 점수나 직접 clip phase는 아니다. 단순 마지막A 글자 비교는 평가횟수 증거로 사용하지 않았다.

새 미션foreground p50/p95/p99=19.957/34.713/36.411ms, 최대52.325ms. 원시읽기 p50/p95/p99=0.0175/0.0194/0.0260ms. 계측·본조회·녹화를 포함하므로64명 비용이나 최적화 효과로 해석하지 않는다. 새8/64 기능회귀·GPU/Present·할당/GC는 미실행이다.

4. 원인 분석

확인: 활성 창만 있는 반복에서도 실제입력이약7.969초 진행하는 동안graph는약5.150초 증가하고 누적기에남지않는 초과분이약2.853초 발생했다. 입력이50ms를넘었을 때graph에50ms만 전달되고 누적기를0으로만드는 이 경로가 느린graph진행에 기여한다는 동일호출 근거가 반복됐다. 모든 클립·모드·GPU단계까지 확대하지 않는다.

기존지원경로와 대조: TimeScale 변수는 고정run 평균속도를 맞추는 반응이 이미A/B/B/A에서 반복됐다. 하지만 정규화 시간증분RMS는약24ms로 줄지 않았고source fit은약14→29mm로 커졌다. LOD0의20Hz유지포즈나이전세대주기표시후보도 이미 부족한 결과를 보였다. 같은방법을 새수정처럼 반복하지 않는다. 현재지원API조사로는 Creature 평가시간과독립적인 표시보간구간 setter를 확보하지 못했다. 엔진전체에 그런기능이 없다는 증명은 아니다.

별도기능추적의 다음표적을 구체화했다. 기존063933의43번은generation1,60.281초에EGRESS→FOLLOW/EGRESS_STALL로 전환했고 최종결과는ARRIVED였다. 전체미종료개체는0이고좁은clearance lease감사위반도0이지만,timeout/handoff 실패라벨이 사라지는 것은 아니다. 최종도착을중간출구성공으로바꾸지않는다. 이사례의접촉표본은컨테이너/본검사차단을가리키며 다음분석에서실제첫걸음거부와handoff계약을연결한다.

5. 검증 수준

반복 검증 완료: 이 빌드·관측 경로의 동일호출 시간 회계 구조가 두 실행에서 일치했다는 범위다. 첫 실행의4background를 포함한 조건 차이는 유지한다. 진단 성공: 새 활성400표본과native/CPU/영상298쌍 연결, 기존지원제어의 한계 대조,43번기능실패의다음사례선정.

미실행/미수용: 지원되는배포수정,원속도회복,지각적부드러움,모든14상태,새8→64기능회귀,64비용/20ms p95,최종수용. 진단반복을기능·시각·성능완료로치환하지않는다.

6. 변경 사항

watch_horde_frame_order_calls.ps1에선택적MinForegroundSeconds와실제watcher소스보존을추가했다.기본값0이라기존호출지연은유지한다.analyze_horde_clock_conservation.py는event ordinal로callback을연결한다.재사용가능한conservation 영상교집합도구와파일명옵션을추가했다.기존같은이름의파일을덮어쓰지않는다.

B8FB/453D/0C6E PBO는변경·재빌드·재서명하지않았으며새실행전3개기존서명검증모두성공.개인키는배포하지않았다.운영서버변경없음.실제실행driver/JS/config는각profile보존본이기준이다.

7. 원시 증거

043225와044620의launch/native/원점bridge/HUD/회계·source proxy·영상연결을해시로연결한다.044620의확정영상·검사이미지·watcher소스·새6초선정계획과파생물도보존했다.기능다음사례는기존063933 state-contract/egress journal분석해시로연결한다.이것은새64명시험이아니다.

기존publish 도구로안전종료·저장전후파일안정성·SHA256·JSON을확인한다.공개패킷은집계와선정실패사례만포함하며전체trace/영상/설치자산/내부경로/프로세스정보를공개하지않는다.

8. 잔여 문제

애니메이션의느린진행원인은이경로에서더구체화됐지만지원되는수정수단은아직미확보다.지원API/그래프소비정책을확보하기전새보간·임의배속·누적기패치를배포하지않는다.후보가생기면평균속도뿐아니라세대경계점프·source적합·공격/착지시계·8부품동기화를함께검증한다.

다음기능작업은43번EGRESS_STALL의동일generation에서현재포즈접촉거부·복구시도·FOLLOW전환을연결하는것이다.기존정상낙하/복구와지원검사는유지한다.HIGH_HOLD13/14/43과stall실패는해결됐다고갱신하지않는다.64비용기여도는별도통제시험으로관리하며20ms p95를계속목표로둔다.

원본 보고서 ID: 2026-10-29_045441_007275+0900_npc-clock-conservation-repeat.md
원본 SHA-256: d736a6ba6cb819287d49163dd3cf6a0a17638d470556f431cd07bbf8827a1dc8
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 집계와기존43번실패사례 — 전체trace아님
{
  "clockRuns": [
    {
      "runId": "runtime-20261029-043225",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "calls": 368,
      "pairs": 367,
      "exactGraphSteps": 367,
      "inputSumMs": 7964.000149164349,
      "graphAdvanceMs": 5249.919891357422,
      "overshootMs": 2747.0000497996807,
      "residualMs": 0.0,
      "graphVsWall": 0.6587663223274876,
      "foregroundCalls": 364,
      "exclusions": {
        "background": 4
      },
      "missionFrameMs": {
        "n": 5214,
        "p50": 20.006,
        "p95": 35.25217,
        "p99": 37.16851499999999,
        "maximum": 64.2406
      },
      "nativeCpuVideoPairs": 237,
      "cpuVisibleCorrelation": 0.9448867333370273,
      "sourceProxyResidualRmsMs": 3.004886712148295
    },
    {
      "runId": "runtime-20261029-044620",
      "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
      "calls": 400,
      "pairs": 399,
      "exactGraphSteps": 399,
      "inputSumMs": 7969.000173732638,
      "graphAdvanceMs": 5149.921417236328,
      "overshootMs": 2853.00000756979,
      "residualMs": 0.0,
      "graphVsWall": 0.646231545163243,
      "foregroundCalls": 400,
      "exclusions": {},
      "missionFrameMs": {
        "n": 5248,
        "p50": 19.9568,
        "p95": 34.712925,
        "p99": 36.411120000000004,
        "maximum": 52.3249
      },
      "nativeCpuVideoPairs": 298,
      "cpuVisibleCorrelation": 0.9484434836839188,
      "sourceProxyResidualRmsMs": 2.8344935263030187
    }
  ],
  "functionalCase": {
    "sourceRunId": "runtime-20261028-063933",
    "selectedFailure": {
      "runId": "runtime-20261028-063933",
      "entityId": 43,
      "eventOrdinal": 623,
      "seconds": 60.281,
      "previousState": "EGRESS",
      "newState": "FOLLOW",
      "transitionReason": "EGRESS_STALL",
      "metadataEvent": false,
      "recoveryGenerationDerived": 1,
      "position": "<4511.519043, 339.181152, 10239.804688>",
      "contactSampleAge": 0.11899999999999977,
      "contactStatus": "NATIVE_JOINT_WORLD_0",
      "contactPhase": "EGRESS",
      "landingDiagnostic": "CLEAR",
      "nativeSameCallSequence": "818",
      "nativeRecoveryGeneration": "1",
      "nativeContactStatus": "StaticObj_Container_1C",
      "nativeMotionElapsedBeforeTransition": "0",
      "terminalOutcome": "ARRIVED"
    },
    "finalArrivals": 64,
    "unresolvedTerminalAgents": [],
    "clearanceLeaseViolations": [],
    "clearanceScope": "Observed start/end lease limits and handoff only. Existing per-step support/contact functions remain the execution gate; sparse logs do not independently verify every sweep. No performance or animation acceptance.",
    "interpretation": "Entity43 eventually arrived, but EGRESS_STALL EGRESS->FOLLOW remains an algorithm failure label; narrower lease audit with0violations is not full movement acceptance.",
    "next": "Trace entity43 generation1 step rejections and timeout/handoff contract; do not relabel recovered timeout as successful egress."
  }
}

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
