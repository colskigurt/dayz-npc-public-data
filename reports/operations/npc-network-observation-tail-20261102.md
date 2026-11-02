## 1. 작업 목표

기존64개체 시험의 EGRESS_STALL 직후 종료가 복구 판단을 가리는지 확인한다. 물리·애니메이션 알고리즘은 바꾸지 않고 누적 milestone 완료 이후 최소20초 관찰한다. 이전 ID55의 정확한 포즈/주변 상태를 복원한 결정적 replay는 아니다.

## 2. 실제 수행

같은765E network PBO와 기존5PBO·64NPC/512parts·fixture·플레이어 자극을 유지했다. 새 격리 mission은 첫 전원 유효상태 도달 후20초를 더 관찰하며 기존 accepted-step/pose recovery/예산/재시도 상태를 읽어 TSV에 추가했다. 새물리질의·본조회·nativehook·시계·그래프·solver변경·영상은없다. 현재 후보의same-NPC28쌍차단은유지하고타NPC/플레이어/월드충돌은끄지않았다.

sixtyfour2-observe는컴파일후알려진PluginConfigDebugProfile 경고의junction/실경로불일치로guard가중단했다. 실패프로필을보존하고,sixtyfour3-observe에서예상경로만정정했다. 모든임의SCRIPT(E)를허용하도록guard를완화하지않았다. Computer Use로반환된DayZ창을활성화하고실제씬스냅샷확인후gate를열었다. 창정지화면은부드러움증거가아니다.

## 3. 측정 결과

|항목|sixtyfour3-observe 단일실행|
|---|---|
|논리NPC/native parts|64/512|
|관찰종료/추가관찰시작|241.463/221.460초|
|실제추가관찰|20.003초|
|누적milestone/유효terminal|64/64 및64/64|
|기존엄격게이트|true;최종failure필드전원빈값|
|계약행/전이|2005/1264|
|전이체인·generation·terminal정합오류|0/0/0|
|FALL→LAND→FOLLOW|87/87|
|명령퇴역detach|512parts 확인|
|실제Present/CPUbusy/GPU/할당/GC|미측정|

이번종료의마지막5초에서failure잔존또는위치고정과접촉거부가동반된개체는확인되지않았다. 이것은이실행의관찰범위이며어떤향후상태에서도정체가없다는증명이아니다.8개체2회통과기록과별개로이번64확장관찰은1회다.

## 4. 원인 분석

확인:기존알고리즘으로64개체의정상낙하·복구·재등반완료가가능했다. 이미완료된시험을즉시종료하지않고20초관찰한뒤에도이번기능게이트는통과했다. 본문에서241초와이전256초의차이를성능개선으로귀속하지않는다. 초기포즈위상/날씨/시작대기가고정되지않았고시험종료규칙과기록필드도다르다.

미확인:이전실행ID55가추가대기만으로회복됐을지는모른다. 이번동일번호55는이전과동일한충돌상태가아니므로정확한재현/수정성공으로보고하지않는다. 이전sixtyfour1의엄격게이트false와ID55마지막5초정체는그대로유효한반례다. EGRESS_STALL 후FOLLOW명칭만으로복구성공을선언하지않는다.

기존checked-landing후보는FALL중이동예산소진을다루며EGRESS후ID55정체와적용조건이다르다. 이번시험에혼합하지않았다. 실제함수/상태가다른문제를같은복구패치로묶지않는다.

## 5. 검증 수준

이번정의된64개체확장관찰:단일기능회귀통과. 전체64개체반복검증완료/이전ID55정체수정/시각품질/성능개선/20ms p95/최종수용:미완료. 과거실패는성공실행으로대체하거나삭제하지않는다. normalQuitVerified=false며launcher종료와게임정상종료를구분한다.

## 6. 변경 사항

stage_scale_observation.py/alias.py,격리mission·런처·읽기전용분석만추가했다. 새PBO는없고기존6PBO해시·서명은실행전검증에성공했다. 원본자산/운영서버/Steam/기존게임소스는변경하지않았다. 이번에새서명했다고기록하지않고개인키는공개하지않는다. network PBO SHA=765E61B0B2E20EA3C7977FF17316808D99D8CDD94DDFBCE7A95F8829CAD6EED8.

## 7. 원시 증거

sourceRunId:animation-network-role-20261101-1600/eight-part-20261102/network-gameplay/sixtyfour3-observe. plan/mission/서버·클라이언트launch·RPT/script·gameplay/milestone/state-contract/terminal/frame/partclock TSV·scale-analysis/observation-tail-analysis JSON을기존체크포인트에해시로연결한다. sixtyfour2-observe 준비실패도보존한다. 공개packet은선정집계이며원시전체/PID/절대경로/키/영상/게임자산은게시하지않는다.

## 8. 잔여 문제

현재후보는아직정식채택보류다. 이전실패의재현과후속실제accepted movement확인이남고,실제화면품질·일반클라이언트호환성·클라이언트/서버분리비용도미확인이다. 물리호스트가추가된8파트경로를서버로비용을옮겨놓고최적화라고하지않는다.

다음물리비용후보로추가7개시각호스트의물리상호작용역할을검토할수있다. SDK에dBodySetInteractionLayer/GetInteractionLayer가존재한다는것만확인됐고실제사용효과·네트워크복제·본평가유지·공격/피격보존은미시험이다. 아직레이어/충돌/시뮬레이션을변경하지않았다. 실제leader/피해receiver/스크립트관절·지지판정은보존해야하며,원본충돌을일괄삭제하는접근은하지않는다. 새측정은후보채택결정에필요한경우만수행한다.

로컬 checkpoint SHA-256: fc993b46852c51453e2d887f4aec74c3914d765e54190e80040ebd5ab90dc874
