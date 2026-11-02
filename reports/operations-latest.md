## 1. 작업 목표

기존 단일 파트 모델 경로 진단을 실제8파트로 확장해 client mode2의 retained buffer·CPU 포즈 진행과 파트의 몸체 위치 정합성을 확인한다. NPC1개체의8파트이며8명 군집 시험이 아니다. 정상 접촉·낙하·추적 기능을 제품에서 변경하지 않고 격리 선행조건을 검증한다.

## 2. 실제 수행

기존7개 렌더 파트의 source/packed LOD 목록을 읽었다. 각각 Geometry LOD가 없는3LOD였다. 시각·Memory·Fire LOD 구간의 원본 바이트를 보존하고, 단일 파트에서 검증한 리더 Geometry LOD만 덧붙인 진단 모델7개를 만들었다. 기존 model.cfg 골격/팔레트 클래스를 상속하고 경로는 역슬래시로 명시했다. ANM·XOB·ASI·그래프·native 시계/LOD 코드는 바꾸지 않았다. 별도 PBO를 빌드·서명·서명검증·packed script/모델 header 검증했다.

기존 공식 진단 client/server·고정 좌표·Run request2·root motion off·AI off 하네스를 사용해 세 조건을 각각1회 실행했다. A는8파트 모두ECE_CREATEPHYSICS, B는리더만ECE_CREATEPHYSICS·나머지0, C는A와같이8파트 초기화 후 같은NPC 내부 파트 쌍만 dBodyCollisionBlock으로 차단했다. SDK dBodyIsSet 가드 뒤 반환된 dBlock을 보존했으며 월드·플레이어·다른NPC의 collision layer나지지/sweep 판정은 건드리지 않았다.

기존 A500 observer의 scheduler/buffer/consumer/interpolate4지점을 byte guard 후 읽기 전용 계측했다. receiver8개까지 anonymous ID로 분리하고5scheduler마다 compressed CPU pose를 표본화했다. 모드/시계/포즈에 수치를 쓰지 않았지만 interceptor hook은 사용했으므로 성능 비교에서 제외한다. Computer Use로 실제 게임 창을 확인했으며 새 녹화는 만들지 않았다. 스냅샷은 몸체 표시를 확인하는 데만 사용했다.

## 3. 측정 결과

|항목|A: 전부 물리 초기화|B: 렌더 파트 초기화 플래그 제거|C: 내부 파트 쌍 차단|
|---|---|---|---|
|factory 반환|8/8|리더1/8,파트1 생성 중 충돌|8/8|
|server45초 DONE|확인|미도달|확인|
|client mode2/animation contexts|8 /8|미계측|8 /8|
|native 관측 오류|0|READY 전종료·미attach|0|
|파트별 표본 / 변화쌍|250 /249|없음|253 /252|
|retained buffer 표본|각250/250|없음|각253/253|
|common Hips 동일 그룹|250/250|없음|253/253|
|server root 최대 거리|3.920080m|없음|0m(390그룹)|
|내부 pair block 반환|없음|없음|28/28|

A는8문맥이 각각 scheduler/buffer/consumer/interpolate1251회를 기록했고C는1268회다. 이는 함수 호출 관측 수이며 평가 FPS·게임 프레임 수·GPU 작업 수가 아니다. A의 표본 관찰 구간은약24.906초,C는약25.204초다.5회마다표본이라 약100ms 간격이며 이 표본 간격을 실제 표시 cadence로 보고하지 않는다.

bufferClock은A에서141회,C에서139회 감소/리셋됐다. 따라서 끝값 차이/벽시계 시간을 애니메이션 배속으로 계산하지 않는다. A500 graph clock은 여전히 미확보다. 공통 Hips 일치는 anonymous receiver그룹의 CPU 채널 일치이며 script part index와일대일 매핑을 완료했다는 뜻이 아니다.

A는389개 온전한 server그룹에서root가약3.92m로벌어져 있었다. C는390그룹에서기록정밀도상모든root가같았고마지막8파트모두NativeSeconds44.9996·updates900·request2·commandActive=true였다. 이 값은 서버 script command 집계이며 그래프 재생시간이 아니다.

8/64 gameplay·14클립 시퀀스·실제 표시 부드러움·Present/CPU/GPU p50/p95/p99·할당/GC·추가 물리host/복제 비용은 미측정이다.20ms p95는 목표다.

## 4. 원인 분석

확인: 모델 계약을 보완하면8실제 파트에서 mode2 retained buffer와CPU 포즈 변화를 동시에 관찰할 수 있다. 그러나 독립 물리 초기화는파트끼리밀쳐root가벌어지는 문제를 만들었다. 같은NPC 내부28쌍만차단한C에서는정지지면root정합성이유지됐다. 이 차단은서로다른NPC끼리밀침을없애는군집최적화가아니며, 하나의표시몸체내부의중복충돌을다루는진단이다. 외부접촉을실제로회귀검증한것은아니다.

B의 생성 실패는A500 RVA0xc0f336에서0참조읽기였다. 앞선 모델미로딩 fault0xc68039와다르다. 물리초기화플래그를빼는것만으로렌더파트를운용할수있다는후보는이조건에서미수용이다. 어떤내부문맥이비었는지타입을최종확정하지않았다. B의클라이언트런처에는server launch JSON공유위반도있었고종료경로로처리됐다. 이운영오류와보존된서버factory충돌을구분한다.

A의추가bone TSV열은string extra를반복문에서명시초기화하지않아파트마다누적됐다. 원본은보존하고A의앞11개안정열만root/상태분석에사용했다. C에서는extra=""로수정해23열로일치했다. 별도native JSON은이문자열문제와무관하다. 같은표본의재분석을새독립반복으로세지않는다.

가설/한계: native network 경로는기존mode0 시간손실을피하면서표시보간을이용할후보지만, 이번8파트에서평균graph속도·실제화면품질을판정하지않았다. 날씨·초기포즈위상·프레임시간을같게맞춘영상A/B도아니다. 추가7물리host와28block의비용이클수있으므로성능목표에유리하다고가정하지않는다. 기존정상NPC파이프라인을네트워크구조로바로바꾸지않는다.

## 5. 검증 수준

8파트 생성·mode2 CPU/buffer 연결: 진단 성공. 내부 pair API 적용·정지지면root정합성: 단일 실행 범위 통과. 반복 검증 완료·이동/전투/등반/낙하 기능 회귀·시각 품질·성능·최종 수용은 미완료다. B는 실패다. 세 조건을 최종 후보3회 성공으로 합산하지 않는다.

## 6. 변경 사항

새 진단 nf_net_parts PBO SHA-256=8BDA6B7A8C95962EF0FDA6BBC6AA228906504C3130D4597070ED2E1E53F7DF9C. 자동 서명·서명검증 및 packed script 일치 성공. 원래7모델의 시각/Memory/Fire LOD 구간 byte보존을검사했고packed ODOL56·LOD4·Geometry해상도header를대조했다. 이검사만으로packed mesh 전체의시각의미동등성을인증하지않는다. 기존3 PBO도런처에서hash/서명을재검증했다. 개인키는패키지와공개자료에없다.

eight-geometry 안의 source/진단PBO·세임무·런처·observer사본·분석만추가했다. originalSource/배포PBO·운영서버·Steam·ANM·XOB·그래프는보존했다. 런처의generic newPbo=false 표식과 달리이묶음에는새진단PBO1개가있음을명시한다. 게임플레이알고리즘은변경하지않았다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/eight-geometry. source-proof.json·packed-model-header-proof.json·build-v3·signature-v3·packed-members-proof-v3와세조건의launch·factory-stages·role-pose·native-observer·heartbeat·RPT/crash·계획을로컬해시목록에연결한다. B의observer 미attach와launch 공유위반을제외하지않는다. SDK enphysics.c·dayzphysics.c의해시를남겨사용한공개API선언을고정한다.

세server/client의launch가terminal이고실제DayZ/PresentMon프로세스가없음을확인한뒤저장했다. 서버harness는DONE후소유프로세스를강제정리하므로정상종료를주장하지않는다. 공개자료는선택집계·CPU표본요약·자체pair차단snippet만제공하며게임자산·원시영상·절대경로·프로세스정보·개인키는공개하지않는다.

## 8. 잔여 문제

다음의가장높은가치는기존mode0 fixture와이mode2 후보의실제표시품질·재생위상을작은동일조건비교로판정하는것이다. 화면이개선되지않으면단지CPU포즈가움직인다는이유로이식을확대하지않는다. 개선가능성이확인돼도이동root연결·외부접촉·피격/등반/낙하·14클립전환과추가물리host비용을별도로검증한뒤8→64로확장한다. 기존scratch 반복성능과착지복구회귀는별도실험으로보존한다. 전체골은active이며기능·시각·20ms p95는독립판정한다.

로컬 checkpoint SHA-256: 981976fb231a5c42ce4fbf1c5c49c9ae8c3f261e7010b5644a01f49a8dafdd56
