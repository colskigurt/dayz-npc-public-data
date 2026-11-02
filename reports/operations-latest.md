## 1. 작업 목표

포즈 기반 복구 V1에서 발견한 기존 terrain-contact ownerON 우회 가능성을 막고, 새 몸통 검사가 실제 발동하는 벽·발판 음성 대조와 V2의8→64 기능 회귀를 검증한다. 기존 게임플레이·정상 밀침·낙하·착지·추적·재등반과 원래 milestone 조건을 보존한다. 성능/시각 품질은 별도 판정한다.

## 2. 실제 수행

V2는 기존TCPolicyRead/TCEnabled/TCTrace의 원래 flag 초기화 정책을 읽고 terrain owner가ON이면 super override로 위임한다. ownerOFF의 pose-core 배치·반경·native/지지/peer/예약/fixture 규칙은 V1과 같다. 소스 역변환 동일성과 literal packed bytes를 각각 검증했다. V2 PBO를 빌드·자동 서명·서명검증했다.

기존 frozen3포즈·6개 motor·22개 상태 guard를 packaged V2에서 재사용했다. forced terrainOFF/ON×poseOFF/ON과 cold flagOFF/ON을 비교했다. 기존 겹침이 있는 컨테이너4면에서는 더 깊은 진입을 시험하고, 별도4면은 native start overlap0/end overlap>0/양수 sweep fraction/fixture0인 입력만 선택해 새 접촉 진입을 시험했다. ON/OFF 각8행이며 ON행은 실제PCUses=1을 요구했다. 반경0.23m·명목4cm 이동은 유지했고 실제 이동은 하지 않았다. 별도기존좌표·현재기록포즈의 LandingSpace 경계 거부도 확인했다.

준비 실패 보존:guards1은 Windows 경로 치환이 누락돼 V1 경로/ V2 hash 불일치를 실행 전 가드가 차단했다. 서버 미시작이다. guards2는 추가 진단식의 indexed minus 구문에서 mission 컴파일 실패했다. 별도 guards3에서 연산자 간격을 수정하고 진단을 함수로 나눠 통과했다. guards4는 fresh wall 입력 대조를 추가해 통과했다. candidate PBO는 이 하네스 수정 동안 바꾸지 않았다.

실제 V2 client/server로8명 후64명을 실행했다. Computer Use로 게임 장면과 전경을 확인한 뒤 기존 start gate를 열었고 영상/native observer는 추가하지 않았다. 원래 전체 milestone·종료 gate를 그대로 사용했다.64실행은360초 정상 시험 한도까지 기록한 뒤 소유 프로세스를 정리했으며 관측 타임아웃이나 임의 조기 중단이 아니다.

## 3. 측정 결과

|검증|결과|범위|
|---|---|---|
|기본OFF·상태 guard|22행 통과|상태 부적합 시 원래 검사 위임, 모든 질의 root 불변|
|기존 owner forced/cold guard|6행 통과|ON에서는poseUses0·delegations1, 기존 거부 유지|
|V1과 ownerOFF parity|frozen6 motor행·22 mode행 bytes 동일|3같은-root 포즈, 독립3반복 아님|
|기존 겹침의 심화 거부|4면×OFF/ON=8행 통과|native fraction0, ON은 실제 새 검사 사용|
|새 벽 접촉 진입 거부|4면×OFF/ON=8행 통과|start overlap0/end1, fraction0.137407~0.362076|
|발판 경계|OFF/ON2행 모두FOOT_Z_SUPPORT 거부|변경 없는LandingSpace, 해당 거부에서pose-core 사용0|
|V2 8명|8/8 완료·8/8 낙하 복귀|67.2155초,136전이·계약 오류0|
|V2 8명 후보 사용|1개체·15회|검사 호출 수, 승인 걸음/CPU시간 아님|
|V2 64명|low64/high milestone63/down0/reclimb0|360.035초, 전체 기존 gate 미통과|
|V2 64명 마지막 상태|64개체 모두FOLLOW·논리Area high|이 값만으로 전체 발판/포즈 재검증 또는 완전 회귀 통과 아님|
|V2 64명 낙하|19/19 LAND·FOLLOW 복귀|기술적climb-end 실패에서 시작한 낙하도 포함|
|V2 64명 계약|656전이·phase/generation/terminal 정합 오류0|기능 milestone 성공과 별개|
|V2 64명 후보 사용|10개체·240회|현재profile terrain ownerOFF,위임0회가 정상|
|64명 server Mission dt p50/p95/p99|51/79/84ms|Present/CPU busy/GPU/표시 cadence 아님|

하강/재등반0은64개체가 그 동작에 모두 실패했다는 뜻이 아니다. HighPassed 전원 조건이 ID4 때문에 열리지 않아 후속 플레이어 자극이 실행되지 않았다. 기존 terminal outcome VALID_TERMINAL0도 이 전체 milestone 정의에 따른 값이다. 측정되지 않은 실제 Present·CPU/GPU·할당/GC·14클립 시각 품질 개선을 주장하지 않는다.20ms p95는 목표다.

## 4. 원인 분석

확인:V2는 기존 terrain ownerON을 보존하고 ownerOFF에서 V1의 frozen 동작을 보존했다. 새로운 pose-core가 실제 호출된 조건에서 새 벽 접촉과 더 깊은 기존 접촉을 거부했다. 이 제한된 fixture/합성 upright pose 대조를 모든 벽·자세·맵의 안전성으로 일반화하지 않는다.

64의 주요 barrier는ID4다.42.29초 첫 등반 시작→43.811초MOTION/SETTLE→44.341초EGRESS/FOLLOW로 완료 횟수1을 남겼다.98.276초low_to_high_lane_2 시도는100.732초 FOOT_Z_SUPPORT의CLIMB_EXIT_VALIDATION으로 FALL 전환됐고101.869초LAND·105.024초FOLLOW로 복귀했다. 이 실제 등반 종료 실패를 지우거나 정상 등반 완료라고 바꾸지 않는다.

109.856초재시도는114.146초지원된 바닥에서 접촉 대기 후SUPPORTED_CONTACT_REPLAN으로FOLLOW에 인계됐다. 이후 높은 영역에서 움직임이 이어졌으며 마지막30초 승인205회·접촉 거부0회·순변위약0.056646m였다. 계속 막혀서 이동0인 사례로 분류하지 않는다. 그러나 authoredClimbs는1이고failure label은SUPPORTED_CONTACT_REPLAN으로 남았다. 원래HighPassed는Climbs>=2와Area high를 동시에 요구해 그 이후 전체자극을 보류했다.

확인된 호출 관계:HoldMotionContact는 bounded wait와 root floor를 확인한 뒤Backoff→FOLLOW로 전환한다. Backoff는Active를 지운다. Climbs는Succeeded와Active가 있는CompleteEgress에서만 증가한다. 따라서 지원된 목표 영역에서 접촉 취소 후 추적을 재개해도 그 자체로 authored 완료가 되지는 않는다. 이 차이를 막연한 패스파인딩 정체나64개체 전체 실패로 보고하지 않는다.

미확인:ID4의마지막4본 포즈 기록은102.02초LAND/low다.114.146초contact 취소 뒤 current-pose LandingSpace/SolveContacts 결과는 기록되지 않았다. Area high·현재 이동 승인·nativeCurveEnded 또는 로그의ExitError만으로 완전한출구 검증을 통과했다고 선언할 수 없다. 특히 거부된Motion의ExitError는 제안한 next의 오차일 수 있어 실제 승인root의출구 거리로 사용하지 않는다.

가설:접촉이 작은 잔여 제안 이동을 막는 순간에도 현재 승인된root가 원래 완료 조건을 만족한다면, 취소 전에 그 현재 상태를 검증해 원래SETTLE→EGRESS를 이어갈 수 있다. 이는 새 후보일 뿐 이번에 구현하지 않았다. 실패한 발판 조건을 무시하거나Climbs를 직접 늘리거나milestone을 완화하지 않는다. V2 호환성 수정이 이번64결과를 유발했다는 인과도 미확정이다. V1과 초기 포즈·날씨·gate 시각·군집 궤적이 고정되지 않았다.

## 5. 검증 수준

V2기존 owner위임·frozen parity·제한된 실제발동 wall/mode/footprint 음성대조:진단 성공/범위한정 통과.8명:단일 회귀 통과.64명:기존 전체 gate 미통과.19/19낙하 복귀는 별도 확인이며 전체하강/재등반 성공을 대체하지 않는다. 반복 검증 완료·일반 기능 수용·시각 품질·성능 수용:미완료다. 이전V1의64/64 단일 성공은 과거 결과로 보존하고 V2의 반복 성공으로 합산하지 않는다. 전체골 active.

## 6. 변경 사항

V2 PBO SHA256=D90E537F1A2828922508B12869EF870F10B1007D9D5451F8F2C00DA17720A211. 자동서명·DSCheckSignatures·packed member bytes 검증 성공. 각 실행 전 기존6PBO+후보1의7개 hash/서명 검증 성공. private key는release/공개물에 없다. 기존6PBO·V1·애니메이션 자산/그래프/native 시계·운영 서버·Steam은 변경하지 않았다.

새진단4폴더·추가guard methods·분석·V2실행기·범위30초 PresentMon helper만추가했다. source-change proof의hash는 newline-normalized text이고 packed proof는 literal bytes라 첫분석의직접비교가실패했다. 두 영역을명시적으로나눠모두대조했고바이트차이를무시한것은아니다. 원시/기존JSON/보고서는보존하고 reviewed 파생본을 별도로저장했다.

server는DONE 뒤하네스정리,client exit-1이므로normalQuitVerified=false를유지한다. Current64용관리자helper는사용자UAC승인을요청했으나Ready/PresentMon/CSV관찰이없었다. 클라이언트종료후오래된바탕화면바로가기는삭제하지않고시험폴더로이동해보관했다. late-process거부가드가구현돼있다는사실과그거부를실제로실행검증했다는주장은구분하며후자는미실행이다.

## 7. 원시 증거

sourceRunId:network-gameplay/{pose-core-candidate-v2,actor24-pose-core-v2-guards1..4,eight-pose-core-v2,sixtyfour-pose-core-v2}. build/sign/packed proof·owner변경proof·준비실패/컴파일로그·원래frozen/mode/owner/world/entryscan/footprint TSV·실행launch·gameplay/state-contract/milestones/terminal/pose-core-use·server-frame·시작전경 확인·소스호출관계·분석/검토JSON을 로컬무결성목록에연결한다. 공개packet은선정결과·ID4전이·자체owner위임/기존완료조건발췌만제공하며원시전체·영상·게임자산·로컬경로·프로세스정보·키는제외한다.

모든대상게임/recorder종료와launch terminal을확인한후파일안정성/JSON/이중hash를검증해저장한다. 보고서를위해시험을중단하지않았다. 실제공개HTTP/hash와supplement schema는별도검증하며외부AI새읽기는미실행이다.

## 8. 잔여 문제

다음단일산출물은접촉취소직전의현재상태를검증하는국소완료후보다. native요청/curve 종료/시간증거·현재승인root의실제exit 거리·target area·full LandingSpace·변경없는lift의전체world/pose접촉·peer/core 조건을동시에요구한다. 통과한경우에만root를옮기지않고원래SETTLE/EGRESS로위임해기존CompleteEgress가완료를집계하게하는가설이다. 하나라도실패하면기존hold/backoff를유지한다. 아직구현/검증하지않았다.

예상이점은이미유효한종료상태의불필요한취소를줄이는것이다. 위험은불충분한종료증거에따른조기완료와추가검사비용이다. 먼저합성/frozen 양성·음성guard와좌표/카운터불변을검사하고,실제native1개체후8→64로검증한다. 취소뒤현재포즈가없으므로이번ID4의정확한포즈재현이나후보효능을미리주장하지않는다. 최초FOOT_Z_SUPPORT 실패의입구/끝점검증도따로남긴다.

시각/비용작업도미완료다. 기존scratch는60조건/평지8과수직63/64까지연구했지만반복성능입증이없고native질의내부버퍼재사용은아니다. 새network실행계층에그대로적용하기전에의존모듈/override와비교조건을대조해야한다. 지금V2의51/79/84ms와과거V1/바닐라Present를빼서개선률을만들지않는다. 사용자관리자승인이필요한실제표시계측은다음살아있는대상에맞춰준비하며새진단툴을반복제작하는것보다기존하네스를우선한다.

로컬 checkpoint SHA256: d197ba6411ff28872d28af1c9d31e7f0fd3b28e301aabc2142323a64e27d3469
