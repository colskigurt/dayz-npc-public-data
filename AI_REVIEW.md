# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T065416-ground-search20-pose-20261029

착지 전후 포즈에 따른 탈출 경로 차이 — 경로2개 재실행,단순 착지 보류 미채택

작성: 2026-10-29T15:52:48.628940+09:00

1. 작업 목표

20번의 고정된 착지 후 포즈에서 짧은 이동·회전을 조합하면 안전한 출구를 찾을 수 있는지 검증한다. 같은 위치에서 낙하 포즈와 착지 후 포즈의 충돌 가능성을 비교하고,기존 Fall 모터에서 단순 착지 보류가 실제 이동을 유지하는지 확인한다. CPU 포즈와 실제 표시/재생 속도는 구분한다.

2. 실제 수행

현재 collision representation과 기록된4개 raw joint를 사용하는 별도 엔진 프로브에서16방향 이동과±1도 회전을 탐색했다. 모든 허용 edge는 기존 PressureEscapeStepClear의 core·예약·fixture 비침투/비심화·world 검사를 거쳤다. 목표는 landing/area·fixture endpoint·ground planning inflation·SolveContacts를 모두 통과해야 한다. 찾은 경로는 초기 상태로 되돌려 모든 edge를 독립 재실행하고 마지막 전체 검사도 다시 했다.

초기 방문 키는 float.ToString의 큰 좌표 과학 표기 때문에1cm 다른 위치를 같은 문자열로 만들었다. 실제 로그에서 legacyRoot/legacyNext가 같고 정수 키는 다른 것을 확인했다. 초기 실행은 폐기하지 않고 물리 경로 결론에서 제외했다. 정수 키 수정 후1cm 및2.5mm 이동을 비교했고 안전한 지면 대조군2개를 추가했다. 방문 격자는 XYZ1mm·yaw0.1도·lift1mm이다.

이후 정확한 touchdown 관측392의 낙하 포즈와129.342초의 착지 후 포즈를2개 root 위치에서 비교했다. 각 쌍은 heading8.8443·lift0.03m·idleOffset0을 같게 했다. 탐색 최대1024확장·4096노드·200동작·1.5m·yaw±45도,남/북2개 heuristic bias였다. 이는 실제 전체 동작 재생이 아닌 통제된 정적 비교다.

마지막 별도4case는 원래 Fall/PressureSlide/AirEscapeTurn을 사용하고,진단용 subclass에서 ground route가 막히면 LandingSpace를 거부하는 조건만 OFF/ON으로 비교했다. accepted footprint X/Z와 기록된 pre-touchdown 높이,dt0.05·fall velocity-15·pressure velocity0·request8·동결 포즈/clock을 사용했다. 배포 PBO·그래프·네이티브 애니메이션 처리 방식은 변경하지 않았다.

3. 측정 결과

수정된1cm 탐색은3개 착지 후 포즈에서 각각6/18/5개의 상태를 탐색하고 open-exhausted로 종료됐다.2.5mm는18/54/15개 상태였다. 이 결과는 설정된 이산 동작/격자 안의 결과이며 모든 연속 경로의 부재가 아니다. 안전한 지면 대조군2개는 즉시 전체 목표 조건을 통과했다. 경로가 없는 행의 replayPassed=true는 재실행 대상 없음의 초기값이라 파생 분석에서는 null로 바꾸어 해석했다. found=false의 goal 값도 이전 case 값일 수 있어 무시했다.

|Root 위치|낙하 포즈,남쪽 bias|착지 후 포즈,남쪽 bias|
|---|---|---|
|touchdown supported 위치|1.08355m·111동작,재실행 통과|9상태 후 탐색 종료,출구 미발견|
|이후 정체 위치|1.01534m·104동작,재실행 통과|6상태 후 탐색 종료,출구 미발견|

두 경로는 회전0회,각각108/102개의 -Z 이동과3/2개의 약한 +X/-Z 사선 이동으로 구성됐다. 모든 이동≤0.04m·yaw step≤1도·총거리≤1.5m와 마지막 전체 검사 통과를 확인했다. 대표 변경 지점과 원시 전체 path SHA를 보존했다. 남쪽 경로는112/105노드 확장과1998/1872개의 후보 질의 후 발견됐다. 질의 수는 전체 physics 호출 수나 CPU 시간과 같지 않다. 북쪽 낙하 포즈2case는1024확장 제한에 걸려 실패/불가능으로 정의하지 않는다.

기존 Fall 모터의 동결4case:낙하 포즈+조건OFF는 첫0.05초에 원래 위치의 LAND로 들어갔으나 전체 출구 조건은 false였다. 조건ON은 root가 바닥보다 약0.1057m 높은 지점에서15초·300회 동안 FALL을 유지하고 lateral step0회였다. 착지 후 포즈의 OFF/ON도300회 동안 이동0이었다. 단순 pre-touchdown route guard는0/4 안전한 복구로 미채택이다.

새8/64 live 안정성·p50/p95/p99·실제 표시 갱신은 미실행/미측정이다. 이전63/64 결과를 변경하지 않는다.20ms는 목표다. 이번에 애니메이션 속도나 부드러움을 고쳤다고 주장하지 않는다.

4. 원인 분석

확인:같은 root·heading·lift에서 현재4joint+fixed-core 충돌 계층의 이동 가능성이 포즈에 따라 달라졌다. 낙하 포즈의 두 안전한 경로가 독립 재실행까지 통과한 반면,해당 착지 후 포즈의 이산 탐색은 작은 영역에서 끝났다. 이는 포즈 전환 시 복구 여유가 줄어들 수 있다는 직접적인 정적 근거다. 스킨 전체나 라이브 애니메이션 전환의 인과를 검증한 것은 아니다.

확인:LandingSpace를 planning 조건으로 미리 거부하면 기존 Fall이 실제 바닥으로 내리는 Accept 전에 반환할 수 있다. 단순 거부는 물리 착지와 다음 포즈 허가를 함께 막아 공중 정체를 만든다. 이 후보를 적용하지 않는다. 안전 경로의 존재만으로 기존 국소 복구 모터가 이를 실행한다고 추론하지 않는다.

미확정:라이브 native pose가 계속 변하는 동안 동일 경로를 통과하는지,해당 pose가 실제 표시에서도 유지되는지,경로 탐색 비용이 어느 정도인지는 미검증이다.4joint 기록은 nativeBefore/After와updates가 같은 touchdown query에서 얻었지만 렌더 buffer/visible-frame 일치의 증거는 아니다. 정적 lift0.03·heading 고정도 실제 원본과의 차이다.

반증/제외:float 키가 합쳐진 초기 실행은 경로 부재 근거에서 제외했다. 단순한 착지 전 경로 거부가 문제를 해결한다는 가설은 동결 모터4case에서 반증됐다. 더 큰 탐색 한도를 사용하기만 하면 해결된다고 주장하지 않는다.

5. 검증 수준

진단 성공:정수 방문 키 오류 분리,포즈별 이동 가능성 차이,단순 착지 거부의 공중 정체. 한정 경로 검증 통과:발견한2개 경로의 모든 edge와 최종 endpoint 재실행. 반복 검증 완료:독립 live run 반복은 아니며 해당 없음. 실제8/64 회귀:미실행. 최종 수용:기능·시각·성능 모두 미수용. 새 후보 runtime 적용 없음.

6. 변경 사항

stage_horde_ground_search20.py,stage_horde_touchdown_admission20.py 및 별도 compile 선택/marker 검사를 추가했다. 변경은 격리 진단 mission에 한정한다. 이전31A286 opt-in guard PBO는 그대로이고 기본OFF·미채택이다. 새 빌드·서명은 수행하지 않았으며,각 프로브 시작 전 기존 PBO 서명 재검증은 성공했다. 운영 서버·기존 원시 로그·이전 보고서·애니메이션 자산/배속/보간은 변경하지 않았다.

7. 원시 증거

search-resolution-analysis는초기 제외 실행과정수 키·1cm/2.5mm·대조군을 연결한다. touchdown-pose-matrix-analysis는2root×2pose×2bias의8행과찾은2개 전체 path를 SHA로 연결한다. 공개 패킷의 pathMoveChangeRows는 시작/방향 변경/끝 지점만 선정한 것으로 전체 path는 아니다. touchdown-admission-analysis는진단용4case와입력 가정을 포함한다.

motor의 admissionRejects는계획 후보와종료 후 관측 질의까지 포함해 실제 touchdown 횟수로 해석하지 않는다. reason/landingGate도종료 후 진단 질의로덮일수있어phase·position·step counter와함께판정한다. generic marker의 case수문구보다실제summary행·키·경로행검사를우선했다. 보고서 저장 전 관련compiler PID 종료·JSON·입력SHA·파일안정성을 검증한다. 공개 자료는경로·PID·키·설치자산을제외한선정근거다.

8. 잔여 문제

다음 통제 실험은물리적 touchdown을기존Floor/Accept로유지하면서다음포즈로넘기는허가를따로보류하는것이다. 그상태에서기존supported recovery와실제native world-contact normal의tangent방향을새제안으로계산하는후보를구분해비교한다. 모든새제안은기존전체step검사를다시통과해야하며거리/시간예산·실패상태를유지한다. 계획은next-experiment-plan.json에저장했고아직구현하지않았다.

진단용best-first 탐색을그대로runtime에이식하지않는다. 현재코드는동결포즈와정규화입력의가능성을확인하는도구이며성능검증이없다. 안전한복구후보가확보되면별도runtime compile·8→64실제회귀·반복과실제표시검증을이어간다. CPU/실제표시시간차이와64명비용귀속은독립실험으로계속남는다.

원본 보고서 ID: 2026-10-29_155248_628940+0900_npc-ground-search20-pose.md
원본 SHA-256: 0be35bee856c1244e6f11815e43ac91dcdea30a4366cbe2a86ef8c66943cdfcf
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selected derived evidence;not full trajectories/rendered animation
{
  "search-resolution-analysis.json": {
    "results": [
      {
        "validation": "controlled-ground-search20-256",
        "admitted": false,
        "rows": [
          {
            "snapshot": "0",
            "bias": "0",
            "expanded": "1",
            "nodes": "1",
            "queries": "18",
            "acceptedEdges": "2",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "0",
            "bias": "1",
            "expanded": "1",
            "nodes": "1",
            "queries": "18",
            "acceptedEdges": "2",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "0",
            "expanded": "3",
            "nodes": "3",
            "queries": "54",
            "acceptedEdges": "8",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "1",
            "expanded": "3",
            "nodes": "3",
            "queries": "54",
            "acceptedEdges": "8",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "0",
            "expanded": "1",
            "nodes": "1",
            "queries": "18",
            "acceptedEdges": "1",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "1",
            "expanded": "1",
            "nodes": "1",
            "queries": "18",
            "acceptedEdges": "1",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          }
        ]
      },
      {
        "validation": "controlled-ground-search20-256-intkey",
        "admitted": true,
        "rows": [
          {
            "snapshot": "0",
            "bias": "0",
            "expanded": "6",
            "nodes": "6",
            "queries": "108",
            "acceptedEdges": "10",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.980469",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "0",
            "bias": "1",
            "expanded": "6",
            "nodes": "6",
            "queries": "108",
            "acceptedEdges": "10",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.970703",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "0",
            "expanded": "18",
            "nodes": "18",
            "queries": "324",
            "acceptedEdges": "42",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.980469",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "1",
            "expanded": "18",
            "nodes": "18",
            "queries": "324",
            "acceptedEdges": "42",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.970703",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "0",
            "expanded": "5",
            "nodes": "5",
            "queries": "90",
            "acceptedEdges": "8",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.980469",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "1",
            "expanded": "5",
            "nodes": "5",
            "queries": "90",
            "acceptedEdges": "8",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          }
        ]
      },
      {
        "validation": "controlled-ground-search20-256-intkey-step25-controls",
        "admitted": true,
        "rows": [
          {
            "snapshot": "0",
            "bias": "0",
            "expanded": "18",
            "nodes": "18",
            "queries": "324",
            "acceptedEdges": "34",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.979492",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "0",
            "bias": "1",
            "expanded": "18",
            "nodes": "18",
            "queries": "324",
            "acceptedEdges": "34",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.970703",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "0",
            "expanded": "54",
            "nodes": "54",
            "queries": "972",
            "acceptedEdges": "138",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.979492",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "1",
            "bias": "1",
            "expanded": "54",
            "nodes": "54",
            "queries": "972",
            "acceptedEdges": "138",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.970703",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "0",
            "expanded": "15",
            "nodes": "15",
            "queries": "270",
            "acceptedEdges": "33",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "0.979492",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "2",
            "bias": "1",
            "expanded": "15",
            "nodes": "15",
            "queries": "270",
            "acceptedEdges": "33",
            "found": "0",
            "goal": "<0.000000, 0.000000, 0.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": null,
            "closestHeuristic": "1",
            "termination": "OPEN_EXHAUSTED",
            "pathReplayApplicable": false
          },
          {
            "snapshot": "3",
            "bias": "0",
            "expanded": "1",
            "nodes": "1",
            "queries": "0",
            "acceptedEdges": "0",
            "found": "1",
            "goal": "<4530.000000, 339.336670, 10240.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": "true",
            "closestHeuristic": "1",
            "termination": "FULL_ENDPOINT_CLEAR",
            "pathReplayApplicable": true
          },
          {
            "snapshot": "3",
            "bias": "1",
            "expanded": "1",
            "nodes": "1",
            "queries": "0",
            "acceptedEdges": "0",
            "found": "1",
            "goal": "<4530.000000, 339.336670, 10240.000000>",
            "travel": "0",
            "depth": "0",
            "replayPassed": "true",
            "closestHeuristic": "1",
            "termination": "FULL_ENDPOINT_CLEAR",
            "pathReplayApplicable": true
          }
        ]
      }
    ],
    "integerKeyDiagnosticVerified": true,
    "legacyKeyAliasWitness": "legacyRoot=4.5115e+06:339172:1.02368e+07:78:27 legacyNext=4.5115e+06:339172:1.02368e+07:78:27 integerRoot=4511504:339172:10236788:78:27 integerNext=4511504:339172:10236798:78:27",
    "conclusion": "Corrected bounded discrete searches find no exit for3 fixed postlanding poses; clear controls pass. Initial float-key run excluded. No proof all continuous routes impossible.",
    "limits": [
      "No-found rows have path replay inapplicable,not passed.",
      "2.5mm run retains100action depth,1.5m travel and256expansions;all six terminate open-exhausted,not expansion bound.",
      "No live or visible quality or performance acceptance."
    ]
  },
  "touchdown-pose-matrix-analysis.json": {
    "runSourceId": "runtime-20261029-143221",
    "buildId": "31A286A3BFC8D8857F70415E9489CD29EE321D0EF8569F3F613629E9868FD126",
    "matrix": [
      {
        "snapshot": 0,
        "rootCase": "touchdown",
        "poseCase": "fall",
        "bias": 0,
        "found": true,
        "travel": 1.08355,
        "depth": 111,
        "rotationEdges": 0,
        "moveCounts": {
          "12": 108,
          "13": 3
        },
        "goal": "<4511.516113, 339.168030, 10235.767578>",
        "replayPassed": true,
        "expanded": 112,
        "nodes": 1690,
        "queries": 1998,
        "pathMoveChangeRows": [
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "0",
            "position": "<4511.504395, 339.172150, 10236.848633>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0",
            "move": "-1",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "1",
            "position": "<4511.504395, 339.172089, 10236.838867>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.00976581",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "9",
            "position": "<4511.508301, 339.171875, 10236.761719>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.087744",
            "move": "13",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "10",
            "position": "<4511.508301, 339.171844, 10236.751953>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.0975097",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "11",
            "position": "<4511.512207, 339.171875, 10236.743164>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.107128",
            "move": "13",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "12",
            "position": "<4511.512207, 339.171844, 10236.733398>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.116893",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "14",
            "position": "<4511.516113, 339.171814, 10236.714844>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.136277",
            "move": "13",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "15",
            "position": "<4511.516113, 339.171783, 10236.705078>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.146043",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "0",
            "bias": "0",
            "ordinal": "111",
            "position": "<4511.516113, 339.168030, 10235.767578>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "1.08355",
            "move": "12",
            "stepVerified": "true"
          }
        ]
      },
      {
        "snapshot": 0,
        "rootCase": "touchdown",
        "poseCase": "fall",
        "bias": 1,
        "found": false,
        "replayPassed": null,
        "expanded": 1024,
        "nodes": 1851,
        "termination": "EXPANSION_BOUND"
      },
      {
        "snapshot": 1,
        "rootCase": "touchdown",
        "poseCase": "postlanding",
        "bias": 0,
        "found": false,
        "replayPassed": null,
        "expanded": 9,
        "nodes": 9,
        "termination": "OPEN_EXHAUSTED"
      },
      {
        "snapshot": 1,
        "rootCase": "touchdown",
        "poseCase": "postlanding",
        "bias": 1,
        "found": false,
        "replayPassed": null,
        "expanded": 9,
        "nodes": 9,
        "termination": "OPEN_EXHAUSTED"
      },
      {
        "snapshot": 2,
        "rootCase": "final",
        "poseCase": "fall",
        "bias": 0,
        "found": true,
        "travel": 1.01534,
        "depth": 104,
        "rotationEdges": 0,
        "moveCounts": {
          "12": 102,
          "13": 2
        },
        "goal": "<4511.512207, 339.167999, 10235.774414>",
        "replayPassed": true,
        "expanded": 105,
        "nodes": 1668,
        "queries": 1872,
        "pathMoveChangeRows": [
          {
            "snapshot": "2",
            "bias": "0",
            "ordinal": "0",
            "position": "<4511.504395, 339.171906, 10236.788086>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0",
            "move": "-1",
            "stepVerified": "true"
          },
          {
            "snapshot": "2",
            "bias": "0",
            "ordinal": "1",
            "position": "<4511.504395, 339.171844, 10236.778320>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.00976581",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "2",
            "bias": "0",
            "ordinal": "3",
            "position": "<4511.508301, 339.171875, 10236.759766>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.0291497",
            "move": "13",
            "stepVerified": "true"
          },
          {
            "snapshot": "2",
            "bias": "0",
            "ordinal": "5",
            "position": "<4511.512207, 339.171875, 10236.741211>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "0.0485335",
            "move": "12",
            "stepVerified": "true"
          },
          {
            "snapshot": "2",
            "bias": "0",
            "ordinal": "104",
            "position": "<4511.512207, 339.167999, 10235.774414>",
            "heading": "8.8443",
            "lift": "0.03",
            "travel": "1.01534",
            "move": "12",
            "stepVerified": "true"
          }
        ]
      },
      {
        "snapshot": 2,
        "rootCase": "final",
        "poseCase": "fall",
        "bias": 1,
        "found": false,
        "replayPassed": null,
        "expanded": 1024,
        "nodes": 1859,
        "termination": "EXPANSION_BOUND"
      },
      {
        "snapshot": 3,
        "rootCase": "final",
        "poseCase": "postlanding",
        "bias": 0,
        "found": false,
        "replayPassed": null,
        "expanded": 6,
        "nodes": 6,
        "termination": "OPEN_EXHAUSTED"
      },
      {
        "snapshot": 3,
        "rootCase": "final",
        "poseCase": "postlanding",
        "bias": 1,
        "found": false,
        "replayPassed": null,
        "expanded": 6,
        "nodes": 6,
        "termination": "OPEN_EXHAUSTED"
      }
    ],
    "conditions": {
      "normalizedHeading": 8.8443,
      "normalizedLift": 0.03,
      "normalizedIdleOffset": [
        0,
        0,
        0
      ],
      "rootPoints": 2,
      "rawPoses": 2,
      "discreteActions": "16 translations1cm and2 turns1deg",
      "depthBound": 200,
      "expansionBound": 1024,
      "travelBound": 1.5
    },
    "conclusion": "Same root/heading/lift, current collision model admits replayed curved escape paths in recorded fall pose;postlanding frozen pose search exhausts9/6states. Pose-transition feasibility matters. Not proof no other postlanding path exists.",
    "limitations": [
      "Frozen4joint/contact representation;not all rendered mesh bones or live animation transitions.",
      "Heading,lift andidle offset normalized;not exact original motion.",
      "No-found rows have replay inapplicable;goal vector output can retain previous case and is ignored.",
      "North-biased fall cases stop at expansion bound,not no-path proof.",
      "No live8/64 recovery,visual smoothness orperformance acceptance."
    ]
  },
  "touchdown-admission-analysis.json": {
    "cases": [
      {
        "pose": 0,
        "mode": 0,
        "rows": 1,
        "first": {
          "pose": "0",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0499878",
          "phase": "LAND",
          "position": "<4511.504395, 339.172150, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "NATIVE_FALL",
          "landingGate": "CLEAR",
          "pressureDiagnostic": "",
          "plans": "0",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "0",
          "wantLanding": "true",
          "fullEndpoint": "false"
        },
        "last": {
          "pose": "0",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0499878",
          "phase": "LAND",
          "position": "<4511.504395, 339.172150, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "NATIVE_FALL",
          "landingGate": "CLEAR",
          "pressureDiagnostic": "",
          "plans": "0",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "0",
          "wantLanding": "true",
          "fullEndpoint": "false"
        },
        "fullRecovery": false
      },
      {
        "pose": 0,
        "mode": 1,
        "rows": 300,
        "first": {
          "pose": "0",
          "mode": "1",
          "iteration": "0",
          "seconds": "0.0499878",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "LANDING_SPACE_BLOCKED",
          "landingGate": "TEST_GROUND_ROUTE_ADMISSION",
          "pressureDiagnostic": "NONDEEPENING_CLEAR",
          "plans": "1",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "12",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "last": {
          "pose": "0",
          "mode": "1",
          "iteration": "299",
          "seconds": "15",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "LANDING_SPACE_BLOCKED",
          "landingGate": "TEST_GROUND_ROUTE_ADMISSION",
          "pressureDiagnostic": "NO_VALID_LOCAL_EXIT",
          "plans": "151",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "2110",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "fullRecovery": false
      },
      {
        "pose": 1,
        "mode": 0,
        "rows": 300,
        "first": {
          "pose": "1",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0499878",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "FALL_NATIVE_JOINT_WORLD_1",
          "landingGate": "CLEAR",
          "pressureDiagnostic": "DEEPER_FIXTURE",
          "plans": "1",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "0",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "last": {
          "pose": "1",
          "mode": "0",
          "iteration": "299",
          "seconds": "15",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "FALL_NATIVE_JOINT_WORLD_1",
          "landingGate": "CLEAR",
          "pressureDiagnostic": "DEEPER_FIXTURE",
          "plans": "20",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "0",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "fullRecovery": false
      },
      {
        "pose": 1,
        "mode": 1,
        "rows": 300,
        "first": {
          "pose": "1",
          "mode": "1",
          "iteration": "0",
          "seconds": "0.0499878",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "LANDING_SPACE_BLOCKED",
          "landingGate": "TEST_GROUND_ROUTE_ADMISSION",
          "pressureDiagnostic": "DEEPER_FIXTURE",
          "plans": "1",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "12",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "last": {
          "pose": "1",
          "mode": "1",
          "iteration": "299",
          "seconds": "15",
          "phase": "FALL",
          "position": "<4511.504395, 339.277832, 10236.848633>",
          "previous": "<4511.504395, 339.277832, 10236.848633>",
          "lift": "0",
          "reason": "LANDING_SPACE_BLOCKED",
          "landingGate": "TEST_GROUND_ROUTE_ADMISSION",
          "pressureDiagnostic": "DEEPER_FIXTURE",
          "plans": "20",
          "steps": "0",
          "separationDistance": "0",
          "admissionRejects": "800",
          "wantLanding": "false",
          "fullEndpoint": "false"
        },
        "fullRecovery": false
      }
    ],
    "fullRecoveries": 0,
    "routeAdmissionGuardAccepted": false,
    "conclusion": "Route-only LandingSpace rejection holds physical root about0.1057m above original floor and does not obtain any lateral step in15simseconds. Do not adopt simple pre-touchdown guard.",
    "limitations": [
      "Frozen poses and forced request8/nativeclock;not live animation.",
      "Counter admissionRejects includes target planning and end-of-iteration diagnostics,not exact actual touchdown attempts.",
      "reason/landingGate may be overwritten by end-of-iteration diagnostic query;phase/position/step counters remain authoritative.",
      "Test input normalizes accepted footprint X/Z and ignores actual pressure velocity."
    ]
  }
}

Own final endpoint predicate
bool DGGroundSearchEndpoint(DGGroundHandoffBody body,DGBTraversalFixture fixture)
{
 vector support;float lift;ref array<vector> contacts={};
 if(!body.Traversal.LandingSpace(body.Agent.Position,support,false) || fixture.Area(support)!="ground")return false;
 if(!body.Traversal.ECEndpointClear(body.Agent.Position))return false;
 if(!DGBGroundRoute.SegmentClear(body.Agent.Position,body.Agent.Position,body.Agent.ObstacleMin,body.Agent.ObstacleMax))return false;
 return body.SolveContacts(body.Agent.Position,body.Agent.Heading,lift,contacts);
}

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
