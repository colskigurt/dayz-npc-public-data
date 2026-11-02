## 1. 작업 목표

실제8파트 network 표시 경로를 기존 GroundBody 인식·경로·접촉·등반·하강·착지 제어에 연결한다. 별도 포즈 재생/위치 지정 fixture를 벗어나 원래 솔버가 NPC 이동을 승인하도록 한다. 초기 범위는1NPC8파트이며8NPC 회귀나64성능 수용이 아니다. 이전 골 턴은14요청·공통본·퇴역 진단과 공개 기록으로 실제 progress를 남겼다.

## 2. 실제 수행

새 default-OFF NFNetworkBody 모듈을 격리 제작했다. 활성화 시에만 기존 B8FB Initialize의 파트 생성 flag/type을 바꿔 복제되는 실제8파트를 생성한다. 리더는 기존 geometry 보유 CrowdLeader 계층,나머지는 앞서 검증한 Geometry 추가 파트다. 같은 NPC 내부28쌍만 dBodyCollisionBlock으로 분리한다. 모든 월드/플레이어/다른NPC collision layer는 그대로다. 선택되지 않으면 기존 Initialize로 위임한다. 후보에는 block 보관용 필드가 추가되어 모듈 로딩 자체까지 무변경이라고 주장하지 않는다.

선언된 편집을 역변환했을 때 줄바꿈 정규화 원본 Initialize 텍스트와 완전 일치했다. Drive/Displace/SolveContacts/Floor/감각/경로/낙하/착지/전투 함수는 교체하지 않았다. server mission은 원래 Director.Update→Crowd.Update→AdvanceFrameFall→기존 Presentation.Sample/accepted logical transform commit 순서를 사용했다. 실제 Goal·기억·NPC 위치를 시험 성공을 위해 직접 설정하지 않았다. NPC world transform 반영은 원래 승인된 논리 위치의 표시 commit으로만 수행한다.

원래 저/고 컨테이너 fixture·motion manifest·목표 자극으로1회,그 뒤 정상 재인식과 재등반을 확인하기 위한 플레이어 자극으로1회 실행했다. 후자는 하강 뒤 실제 loss가 생긴 후 플레이어를 NPC 시야 앞5m에 두고, LOCAL_VISUAL 재인식을 기다린 다음 실제 플레이어를1.5m/s로 컨테이너 남쪽 진입부까지 유도하고 낮은 지붕으로 이동시켰다. NPC ConfirmedTarget·SeenAt·LastSeen·경로·행동 상태에는 값을 쓰지 않았다.

클라이언트 실행을 안내하고 Computer Use로 live3의 실제 컨테이너·NPC 화면을 확인했다. live4 화면 확인 시도 시에는 시험이 이미 종료되어 새 창을 조작하지 않았다. 네이티브 관측기·새 녹화·덤프는 만들지 않았다. 날씨/전경/시각 조건을 맞춘 성능 A/B가 아니다.

## 3. 측정 결과

|항목|live3:원래 자극|live4:재인식 유도 자극|
|---|---|---|
|논리 개체/파트|1/8|1/8|
|시험 종료까지|약150초|약42초|
|이동 거리|35.5476m|63.2843m|
|최종 인쇄 climb/drop|2/1|3/1|
|기록된 landing 최대|1|1|
|발견/놓침|1/1|2/1|
|기록된 경로 계산/실패|7/0|25/0|
|기록된 접촉 solve/실패|3014/0|884/0|
|기록된 joint sweep|12056|3536|
|기록된 foot ray|30140|8840|
|post-commit 파트 최대 거리|0m|0m|
|같은NPC 내부 block|28|28|
|3등반+하강+low FOLLOW 종료 predicate|false|true|
|8명령 retirement detach|true|true|

카운터는 마지막0.2초 희소 표본이며 정확한 최종 총합이 아닐 수 있다. CPU 비용 단위나 개별 native API self-time도 아니다. 서로 다른 자극·길이이므로 카운터 차이를 최적화 효과로 계산하지 않는다. 최종 predicate는 보존한 mission 분기와 실행 결과 인쇄를 연결해 확인했다. live4 마지막 TSV행은 terminal 직전 EGRESS·climbs2이므로,최종 FOLLOW·climbs3의 직접 TSV snapshot은 누락됐다. 다음 시험에는 종료 판정 직후/retirement 전에 같은 호출의 전체 최종 상태를 남겨야 한다.

live3는 두 등반·하강·착지 후 지면 FOLLOW로 돌아왔지만 마지막으로 본 지면 목표에 도착해 SEARCH→IDLE/NO_CURRENT_MEMORY가 되었고 재등반하지 않았다. 이를 성공으로 바꾸지 않았다. live4는20:04:06.649 앞쪽 배치→20:04:07.745 LOCAL_VISUAL 재인식→20:04:24.063 지붕 목표→20:04:27.786 functional=true/climbs3/drop1 결과였다. 정적 source와 지면/마지막 목표 타임라인은 live3 대기가 경로 실패가 아닌 감각·기억 조건과 일치함을 뒷받침하지만,live3에는 상세 sight angle/reason을 계측하지 않아 FOV가 유일한 원인이라는 결론은 보류한다.

두 실행 모두 server DONE 후 harness가 소유 서버를 정리했고 client는 강제 종료 없이 exit=-1이었다. normalQuitVerified는false다. 이번 p50/p95/p99·Present·CPU busy·GPU·할당/GC·전경 비율은 미측정이다.20ms p95는 목표다. playerContacts는live4에서0으로 외부 플레이어 밀침이 실제 발동했다고 주장하지 않는다.

## 4. 원인 분석

확인: 네트워크 파트에서도 실제 리더 command의 root delta가 원래 물리/경로 제어에 공급되고,그 승인된 이동으로 등반·하강·착지·재인식 뒤 재등반까지 연결될 수 있었다. Foot ray/sweep/접촉 검사는 계속 실행됐다. 포즈만 재생하거나 NPC를 경로 좌표로 강제 이동시킨 결과가 아니다. mode2 factory/자산 계약은 이전 계측과 동일한 계보지만 이번 실행에서 native mode/buffer 값을 다시 후킹하지는 않았다.

확인: 첫 실행의 재등반 중단은 마지막 기억/수색 종료 상태와 함께 발생했다. 후속은 플레이어 자극을 바꿔 정상 LOCAL_VISUAL 경로를 다시 작동시킨 것이다. AI를 수정해서 고친 OFF/ON 성능/기능 개선 실험이 아니다. 하나의 원인을 모든 시야 상실 상황에 일반화하지 않는다.

준비 실패: live1은 mission OnUpdate 인자명이 엔진 prototype timeslice와 달라 컴파일 거부됐다. live2는 새 mission 경로를 실제 실행 junction 대신 해석된 workspace로 기록해 exact known-plugin warning guard가 거부했다. live3에서 실제 로그 경로로 맞췄다. 오류 guard wildcard·SCRIPT(E) 일괄 허용은 하지 않았다. 준비 실패2건은 실제 기능 실행/독립 반복에 포함하지 않는다. staging helper의 Windows 경로 escape SyntaxError도 실행 전 실패로 보존한다.

가설/위험: 같은NPC 내부 충돌만 제거해도8물리 host는 남는다. 다른NPC 사이 물리와 기존 scripted crowd가 중복되거나 표시 파트가 외부 물리 비용을 늘릴 수 있다. 이는 비용 개선이 아니라 새 검증 대상이다. 공식 dBodySetInteractionLayer API가 존재함은 읽었지만 사용하지 않았다. 월드/지지/캐릭터 충돌을 무근거로 없애는 변경을 넣지 않는다. 전체 Initialize 사본을 쓰므로 기반PBO hash/정규화 역변환 검증 없이 다른 버전에 적용해서는 안 된다.

## 5. 검증 수준

새 모듈 빌드·서명·packed source·engine compile: 통과. 한 개체 실제 추적·2등반·하강·착지: 두 자극에서 제한 진단 성공. 정상 재인식·세 번째 등반 종료 predicate: live4 단일 성공,직접 최종 상태 기록의 공백은 명시한다. 동일조건 반복 완료·정상 전투/피격/사망·플레이어 밀침·타NPC 접촉/자연낙하·8/64 전체 회귀·시각 품질·성능 최적화·최종 수용은 미완료다. 전체 골은active다.

## 6. 변경 사항

새 nf_network_body PBO SHA-256 765E61B0B2E20EA3C7977FF17316808D99D8CDD94DDFBCE7A95F8829CAD6EED8,6040bytes. 자동 서명·서명검증 성공. World/Mission script2개 packed member가 source와 일치했고 실제 engine에서 컴파일·실행했다. 기존5PBO까지 총6개 signature/hash를 각 실행 전에 검증했다. 개인키는 배포/공개에 포함하지 않았다.

network-gameplay 안의 default-OFF source/build·live1~4 격리 프로필·분석·보고서만 추가했다. 원본 source·기존 서명 PBO·운영 서버·Steam·원본 ANM/XOB/ASI/그래프·native 시계·LOD 구현은 보존했다. 새로운 작은 PBO의 camera는 fixture 관찰용이며 기본 파이프라인의 렌더러/보간 교체가 아니다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/network-gameplay. source-equivalence,build-v3,packed-members-proof-v3,signature-v3,실제6PBO launch signatures와live1~4 mission/launch/RPT/script/guard·gameplay-body/analysis·batch-results를 로컬 해시 목록에 연결한다. live3/4는 각1실행이며 서로 다른 자극이다. 이미 수집한 source/로그를 다시 분석해 독립 횟수를 늘리지 않는다.

공개 packet은 자체 초기화 변경 범위·재인식 자극 함수와 대표 상태 전이/집계만 제공한다. 로컬 절대경로·프로세스 정보·키·게임 자산·원시 전체 로그는 공개하지 않는다. 큰 영상·ETL·덤프를 새로 만들지 않았다.

## 8. 잔여 문제

다음 가장 가치 있는 산출물은 기존 하네스를 활용한8개체 network 접촉/낙하/재등반 시험 결과다. 시작 전 개체ID별 prev/new/reason/recovery-generation과 종료 snapshot을 기존 계약 로그에 연결한다. 좁은 지붕에 전원이 같은 순간 서는 조건을 요구하지 않고,각 개체의 유효한 도착/정상 낙하 후 지지·FOLLOW 복귀·재등반 진행을 누적해 구분한다. 영구 FALL·무효 상태 전이·해소되지 않는 출구 정체는 실패로 남긴다.

외부 접촉이 확인되기 전 전투/피해/시체까지 네트워크 후보에 포함했다고 보고하지 않는다. 새 collision-layer 변경을 같은 실행에 섞지 않는다. 추가 물리/복제 비용은 별도 동일조건 baseline/candidate 반복으로 측정한다. 기존 scratch·SupportFast 연구는 보존하고 이 기능 시험을 성능 개선 근거로 쓰지 않는다.8개체 안전/비용 문턱을 확인한 뒤64로 확대한다.

로컬 checkpoint SHA-256: 869bc4da0cc00231b84a75880f7ea614e330300fafa4a6f12af87775c6cb202e
