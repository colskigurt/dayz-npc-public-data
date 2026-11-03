> 최신 작업: 방향 유지 후보8/8 통과·64는53/64 실패. 낙하82/82 복귀, 고정14명 정렬 전환 표본156→0. 갱신당 지지 비용은 비슷하며 시각·성능 미입증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-entry-facing-regression-20261103.md).

> 최신 작업: 입구 방향 후보: 물리22회·범위36회·기존66행 대조 통과. 약2cm 보정과 실제 발판 끝 차단 확인. 라이브8/64·시각·성능 미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-entry-facing-physics-20261103.md).

> 최신 작업: 재등반14명에서정렬전환156표본확인.복구억제가설제외.입구facing유지격리후보서명완료,실제엔진/기능/시각/성능미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-entry-facing-candidate-20261103.md).

> 최신 작업: 접근후보8/8통과·64는46/64실패.낙하81/81복귀·실제6명110사용.후보미채택,재등반이력우선분류.시각/성능미측정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-approach-regression-20261103.md).

> 최신 작업: 접근후보동결3자세OFF0→ON10step·0.303m.기존46guard동일·새20음성통과.라이브8/64·시각·성능미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-approach-frozen-controls-20261103.md).

> 최신 작업: 접근복구!Active경계확인.회전속도조정제외.격리후보서명완료,엔진/효과/시각/성능미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-head-approach-candidate-20261103.md).

> 최신 작업: 골반후보8/8기능통과·64명49/64실패.실사용7낙하모두복귀,전체81/81.미충족13FOLLOW·2종료직전SETTLE구분.효능인과·시각·성능미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-hip-contract64-outcome-20261103.md).

> 최신 작업: Move RTTI/생성→공통manager의2조건부 갱신 연결.지속 scalar는MovementSpeed/LookDir 입력필터로 구분.지지 생산/접촉재사용은 미확인.새게임 없이30초1개체 관측 필요범위 제시. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-vanilla-move-stage-trace-20261103.md).

> 최신 작업: 바닐라 native Move/API·101 getter 경계 확인.지지 함수/형상/cache/주기 미확인.기존 Fast/Scratch 중복 정리,AnimalBase drop-in 대체 근거 없음.새 실행/구현 없이 소유권 명세 우선 권고. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-vanilla-controller-review-20261103.md).

> 최신 작업: 순차 포즈 재현에서 기존2조건 착지 실패·골반계약 후보 착지 통과.비적격14조건 보존·서명 대체PBO 실제 OFF/ON 검증.새8/64·시각·성능 미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-hip-sequence-package-20261103.md).

> 최신 작업: 골반 검사 반경 한 요소 비교:12포즈 모두 첫 이동,착지+전체포즈는1/12.기존24결과 재현·네 벽면 거부.미션 진단만,미채택·성능/시각미측정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-hip-contract-diagnosis-20261103.md).

> 최신 작업: 근지면 후보64명52/64 실패.실제사용7낙하 중6복귀·1장기차단.63명 현재FOLLOW와 이력 실패를 구분.복수접촉 악화 확인으로 최저깊이/역스윕 보정 기각,제한탐색 경로 미확보. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-near-ground-fall64-contact-20261103.md).

> 최신 작업: 근지면 FALL 포즈 복구: frozen3cm 후 원래 착지 통과·12위임가드·4벽 거부. 새PBO 서명검증 성공.8명 8/8,실제후보사용0회.64/성능/시각 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-near-ground-fall-20261103.md).

> 최신 작업: 64명 OFF/ON 각2회:4/4 기능 실패,서버p95 이득 방향 불일치로 후보 미채택. 장기 낙하 위치의 FOOT_X_SUPPORT 거부 재현;10cm 이웃 착지점 통과는 경로 성공 아님. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-slot-support-four-20261103.md).

> 최신 작업: 64명 첫 OFF/ON: 유효 종료58/39명, 서버p95 121/124ms. 검사 수 감소에도 성능·기능 개선 미입증. ON 장기 착지 차단1명 보존, 반복 진행 중. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-slot-support-first-pair-20261103.md).

> 최신 작업: 조건부지지검사ON8명:8/8·낙하복귀12/12·실제fast8/8.새64OFF/ON/OFF/ON비교는진행중.성능·시각·20ms목표는미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-slot-support-safety-20261103.md).

> 최신 작업: 그래프 범위 후보8명 통과·64명 실패(유효label40/64). 반복경고는각1건.64명명령시간0.643배·서버dt p95 116ms 관측,Present/시각/성능성공은미판정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-slot-regression-clock-20261103.md).

> 최신 작업: behaviorSlotID 선언0~31에 native 값-1이 들어가 변수 번호가 손상되는 경로 재현. 최솟값-1 후보는1NPC·8파트 그래프 위치44/44·수신 종료128,해석8/8통과.8/64기능·시각·성능은 미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-graph-slot-bounds-20261103.md).

> 최신 작업: 그래프 데이터 진입bit5는 같지만 서버38비트·클라이언트23비트 소비. 부모 필드 이전15비트 불일치 확인. 원인·수정·성능은 미확정. c0bc40 guard-only 설명 정정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-graph-prefix-contract-20261103.md).

> 최신 작업: 같은16바이트에서 부모 필드는 서버 bit126(update0), client bit47(update1/presence1). 79비트 위치 불일치 확인. 원인과 수정은 미확정, 성능 미측정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-parent-field-cursor-20261103.md).

> 최신 작업: 서버개체emitter의16byte생성확인,client유효전체와8/8동일.공유codec계약불일치원인추적중.3함수정적분석완료,새성능/시각결과없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-record-producer-20261103.md).

> 최신 작업: 사전연결관측:정상111B/비정상16B,8건모두146bit까지읽어18bit초과.잘못된부모ID는범위내에서먼저해석됨.생성/해석계약원인추적중;성능미측정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-parent-record-bounds-20261103.md).

> 최신 작업: 부모 입력16건 raw bits와인자일치:정상2bit/비정상99bit.초기writer111byte 확보,수신길이관측은초기시점놓쳐미확인.새성능/시각결과없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-parent-bitcursor-20261103.md).

> 최신 작업: 1개체8파트: 서버 부모snapshot0, 클라이언트 같은8 receiver에서 첫ID0→다음동일비정상ID 전환 확인. 생성후 입력경로로범위축소;원인/성능미확정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-parent-input-transition-20261103.md).

> 최신 작업: Animal/Infected 직렬화에서 공통 부모 처리까지 정적 호출 연결. 실제 오류 패킷 대응 미확인으로 초기화·ID강제 수정 보류. 새 성능/시각 결과 없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-creature-serialization-20261103.md).

> 최신 작업: 조건부지지래퍼2독립비교:지붕약24.4~25.0%감소·지형약0.6~1.6%증가(메서드만).3native함수확보,전체프레임/시각미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-context-method-reader-20261103.md).

> 최신 작업: 서버8파트부모NULL·client등록8건실제부모NULL과미해결계층참조대조. 공식AddChild/GetParent계약연결;송수신원인·비용미확정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-parent-contract-20261103.md).

> 최신 작업: 네트워크최초등록8건에동일미해결ID확인·Ghidra17분분석완료. 재삽입loop확보,전송/해석원인미확정·성능미측정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-registration-origin-20261103.md).

> 최신 작업: 네트워크 미해결ID 반복조회 caller 확인. SETUP OFF/ON 모두8파트·경고최대200/초로후보미채택. 성능/시각미측정,정적분석대기. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-retry-diagnosis-20261103.md).

> 최신 작업: 상태 제한 지지 후보:19가드·8명8/8·64명64/64,낙하 복귀82/82. 실제 비용/시각 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-support-context-8-64-20261103.md).

> 최신 작업: 기존 SupportSweepFast 2독립 비교:지붕 메서드 약26% 감소·지형 약6.6% 증가. 전역 수용 기준 미통과. 상태 제한 후보는 명세만,8/64·시각·전체 프레임 미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-current-support-method-cost-20261103.md).

> 최신 작업: 현재 코드의 3팔 배열 재사용 비교:2독립 실행·288000회. 국소 중앙값 약1.5~2.5% 감소지만 사전5% 기준 미달. full-game/시각/20ms 미검증, 주력 후보로 반영하지 않음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-current-scratch-method-cost-20261103.md).

> 최신 작업: 검증된 작은 높이 보정 후보:50가드 통과·실제ON/OFF 대비=True.8명8/8·64명64/64 단일 회귀.시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-contact-end-bounded-lift-20261103.md).

> 최신 작업: 실제 native 완료 경로1회 활성화.지속 접촉 OFF/ON은 모두취소:17행에서 약3.815mm lift 차이를 거부하는 가드 확인.미해결·시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-contact-end-real-player-core-20261103.md).

> 최신 작업: 현재 위치 등반 종료 검증·40가드 통과.8명 8/8,64명 64/64 단일 회귀.실제 후보 사용0으로 효능 미검증·시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-contact-end-current-state-20261103.md).

> 최신 작업: V2owner위임·벽/발판guard통과,8/8회귀통과.64는ID4contact취소후완료집계1로high63에대기,전체미통과.마지막64FOLLOW/high와전체통과를구분. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-pose-core-v2-guards-20261103.md).

> 최신 작업: 포즈기반복구후보V1:ID24고정입력이동·8/8·64/64단일기능통과,91/91낙하복귀.기존terrain ownerON호환성발견·V2소스만준비.시각/성능미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-pose-core-recovery-20261103.md).

> 최신 작업: ID24 일반4관절과복구고정몸통의허용공간차이확인.16방향효과없음·1cm도약1.95cm후재정체.단순후보미수용,기능/시각/성능해결주장없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-actor24-overlap-diagnosis-20261103.md).

> 최신 작업: 기록포즈에서머리충돌과기존EC탈출계획확인.8/8회귀통과·64EC복구7회완료이나ID24입구정체로높은면63/64·전체미통과.성능미입증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-existing-egress-integration-20261103.md).

> 최신 작업: 8NPC 시각호스트 NOCOLLISION 후보는파트참조소실로기각. 동일관측기OFF는8/8·복구9/9. 시각/성능개선은미입증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-render-host-layer-rejection-20261102.md).

> 최신 작업: 64NPC/512parts 추가20.003초관찰에서64/64 엄격기능단일통과·낙하복귀87/87. 기존ID55실패는보존;수정/시각/성능성공은아님. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-observation-tail-20261102.md).

> 최신 작업: 64NPC/512parts단일시험:이동이력64/64이나엄격기능게이트false. ID8이력잔존/ID55최종5초정체구분. 서버timeslice p95=66ms;Present/화면품질/성능개선미판정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-sixtyfour-audit-20261102.md).

> 최신 작업: 실제8NPC/64parts:독립2회8/8terminal·낙하18/18LAND/FOLLOW·288전이/gen일치. historical EGRESS_STALL 각1회복구.64NPC/최종시각/성능은미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-eight-regression-20261102.md).

> 최신 작업: 네트워크8파트를 실제 GroundBody 제어에 연결.1NPC:2등반/하강 뒤 기억종료 대기 보존,다른 자극에서 LOCAL_VISUAL 재인식·3번째 등반 단일 성공.8/64·외부접촉·시각/비용 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-gameplay-pilot-20261102.md).

> 최신 작업: 실제1NPC8파트 mode2:14요청 전달·548공통본/기준위치 묶음 정합·2세대 명령해제 후삭제.20초 전경 영상 확보.게임플레이/최종시각/64성능은 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-network-clip-sequence-20261102.md).

> 최신 작업: 기존 연구 중복 검토: scratch 성능 미확정·착지8/64 단일 회귀. 전경 통제 실제8파트 영상에서mode2 유망하나 품질/안정성/64성능 채택 보류. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-research-direction-update-20261102.md).

> 최신 작업: NPC1개체8파트 mode2·buffer/CPU 포즈 연결. 내부28쌍 충돌 차단으로 정지root 정합성 단일 통과. 렌더물리초기화 제거는 충돌 실패. 시각/14클립/64성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-animation-eight-part-mode2-20261102.md).

> 후속 종료 상태: 모델 경로 진단 유지. Ghidra는 자체15분 초기화 시간초과로 종료·결과 미확보. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-animation-factory-analysis-close-20261102.md).

> 최신 작업: Geometry 후보의 무효 시험 정정: 실제 모델 미로딩 확인. 경로 구분자만 수정해 native LOD4/Geometry3·단일 파트 생성/10초 명령 통과.8파트·시각·성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-animation-factory-model-path-20261102.md).

> 최신 작업: 실제8파트 NPC 활성화 OFF/ON 한 쌍. OFF 정체·ON 착지/추적 재개. 반복/시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-checked-landing-activation-20261102.md).

> 최신 작업: 검사 경로 기반 착지 복구 후보.64 단일 기능 통과=True, 반복/시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-checked-landing-corridor-20261102.md).

> 최신 작업: scratch8/64 수직 회귀.8명 통과·64명63/64, 착지 복구 실패 고정 재생. 시각/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-scratch-vertical-recovery-20261102.md).

> 최신 작업: 물리 질의 호출 소유권 확인 및 scratch 국소 후보. 서명 검증·60조건 동등성·평지8명 추적 통과. 성능·수직64·시각 품질 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-query-scratch-pilot-20261102.md).

> 최신 작업: 기존 연구 중복·수정 가능 후보 검토 완료. SupportFast64 미수용, 시간 보존 후보는 격리 진단 한정. 새 구현/실행0회. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/npc-research-review-20261102.md).

> 최신 작업: Days Gone 기존 CPU 스택 재분류. REA 전체 초기화 완료; 물리·렌더·UI 일부 확인, 원본 애니메이션 비용 미확정. 새 실행0회. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/days-gone-stack-attribution-20261102.md).

> 최신 작업: Days Gone 지붕 고정20명75초. Present p95약22.1~22.3ms. DayZ64와 조건이 달라 직접 배수/animation selftime은 미판정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/days-gone-stationary-20261102-1231.md).

> 최신 작업: DayZ64 상세 비용4반복·발판 소실 복구1회. 이동/밀침별 접촉과 물리 질의 스택 비용을 좁혔다. 애니메이션 표시 품질·세부 native self-time·20ms는 미완료. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/detailed-cost-attribution-20261102.md).

> 최신 작업: 바닐라/커스텀1·8추가 및 기존64비교. 접촉 호출량과8파트535본, 단일개체 CPU시계 손실 구분. native selftime/개체당 순수단가/시각품질 미확정. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/per-agent-resource-20261102.md).

> 최신 작업: 같은 평지·카메라·입력의 V64/C64 독립2회. 커스텀 전체 비용 증가 및 CPU animation 시계 손실 측정, 시각 품질/20ms 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/matched-flat-pursuit-20261102.md).

> 최신 작업: 바닐라 평지 추격의 실제 CPU/GPU/Present 측정. 새2회, 두번째 sampler 약3.84Hz. VSync/표시 주사율 영향과 커스텀 조건차를 분리하며20ms/시각/animation self-time은 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/vanilla-flat-pursuit-20261102.md).

> 최신 작업은 외부 참고자료 검토다. 새 게임실험/측정 없음. WWZ 배경 VAT와 전경 캐릭터를 구분하고 DayZ 적용 제약을 정리했다. [검토 보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/wwz-animation-reference-review-20261101.md). 기존 애니메이션 진단·실험 수치와 미수용 상태는 유지한다.

> 최신 진단: painted HUD 막대해독은 확인했으나 반복이미지를 제외한 직접독립ID30/156개로 두실행 모두시간coverage미수용이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-painted-label-20261101.md). 외부6개호드자료는README/listing만검토했고구현에반영하지않았다. 기존NPC PBO유지,시각·성능미수용.

> 최신 진단: client TickCount872개/QPC bracket·수동HUD9개(직접native구간8개)·고정점 projection을 연결했다. binary막대 미표시로 전체자동frameID는미완료이며 output-age를NPC/물리표시지연으로취급하지 않는다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-frame-label-20261101.md). 새계측PBO만추가·기존게임PBO유지,시각·성능미수용.

> 최신 영상 분석: 기존931프레임을 재사용했다. 두 색상 기준이 함께 어깨를 머리로 오검출한 사례를 확인했다. 탐색모델의50ms시프트는 실측 렌더지연이 아니며 정확한 표시지연은 미확정이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-optical-association-20261101.md). 새runtime/PBO/영상0, 시각·성능미수용.

> 최신 진단: 자연 캐시 갱신1,088건에서dirty1→0·행렬변화를 확인했다. OBS1,443프레임/QPC연결은 성공했으나 game texture나이·본표시지연·부드러움은 미확정이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-cache-visible-20261101.md). 게임 코드/PBO변경없음, 시각·성능미수용.

> 최신 진단·명칭 정정: A500 GetBonePositionMS는slot618이며이전진단slot610 MS명칭은철회한다(일반exe610은유효). 15.4939초772표본에서query/interp의skeleton·변환·행렬저장소alias를확인했다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-pose-identity-20261101.md). dirty1은자동결함판정이아니며실제표시시각은미측정이다.

> 최신 진단: A500 mode2 실제4경로689회·25.2109초229표본·오류0을확보했다. query/interp값229/229같지만pointer identity·좌표공간은미확인이다. 실제화면은미확보다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-trace-20261101.md). CPU 변화만으로시각/성능성공을판정하지않는다.

> 최신 진단: A500 전용 scheduler·buffer·interpolation과 객체 필드를 정적으로 연결했다. animation context+628/mode+670/subobject+600이며 일반exe 오프셋을재사용하지않는다. graphClock writer는미확인이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-diag-map-20261101.md). 새runtime·시각/성능수용없음.

> 최신 진단: 실제 PowerShell5.1의 JSON 배열 중첩이 하네스 중단 원인임을 재현했다. 수정 guard11/11 통과. 진단exe의1회 미션DONE·조기disconnect0·BE kick0을확인했다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/diag-guard-20261101.md). 일반exe 해결·시각/성능수용과는구분한다.

> 최신 진단: native 관찰기 없는 실행에서도0x000400F0/BattlEye restart-required 화면과서버kick사유를확인했다. READY후32.170초이탈. 진단exe3시도는NPC생성전하네스중단이며연결검증이아니다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/no-observer-error-20261101.md). client상태·내부원인·시각/성능수용미완료.

> 최신 진단: 공식 런처 직접 실행 분기를 확인했지만 native 관찰기 없는 기준 실행도37.331초 뒤 이탈했다. 서버 DONE보다7.386초 앞섰으며 이번 오류 코드는 미포착이다. BEClient 모듈·갱신 로그 확인. client 비활성 상태·이전 오류와 동일성 미확인. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/launcher-test-mode-20261101.md). 기능·시각·성능 개선 미수용.

> 최신 진단: 마지막 월드 이탈에서0x000400F0 BattlEye restart-required경고를 확보했다. matrix OFF도관찰종료뒤예정서버종료전에이탈해matrix읽기만의문제로단정할수없다. client실행상태확인전추가native시험보류. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/network-continuity-20261101.md). 시각·성능 개선 미수용.

> 최신 진단: live 이름hash→slot과부분6점pose대응을확인했다. 수정계측123표본의유효구간은7.286초로25초연속gate실패다. 기존실행본은이미opaque signedID계약을지켰으며새계측오류만수정했다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-network-pose-20261101.md). 전체포즈·시각·성능미수용.

> 최신 실행 진단: 연결 시도3회 중 마지막1회에서 실제 mode2/retained buffer/1273호출을 포착했다. graph/QPC 비율1.000053이나 client 요청·포즈 동등성과 시각·성능 개선은 미검증이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-network-role-20261101.md). 실패 기록 보존.

> 최신 정적 진단: client mode2는 별도 buffer 시계를 거쳐 mode0과 공유하는 pose 소비자로 연결된다. 실제 네트워크 역할 비교와 요청 동등성은 미실행이다. 시각·성능 개선 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-client-mode2-20261101.md). 이전 기록 보존.

> 최신 진단: 실제 Animal selector6회에서 빈 참조 목록과LOD2를 확인했다. headless mode1의200ms 경로는 설명됐으며 client mode0 시간 손실과 구분한다. 시각·성능 개선 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-lod-selector-20261101.md). 이전 기록 보존.

> 최신 진단: 공개 LOD0 이후 내부 LOD2 dispatcher6회·interval writer를 포착했다. native caller의 가상 selector 슬롯+0xb10까지 연결했으며 실제 입력과 지원 제어는 미확정이다. 시각·성능 개선은 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-precision-writer-20261101.md). 이전 기록 보존.

> 최신 진단: 자동 LOD/생성시 LOD0을 A/B/B/A로 비교했지만 안정 command 갱신은 모두 약200ms. 별도 probe에서 LOD0 호출 후50ms 필드가173ms뒤200ms로 재설정됨. 정확한 writer는 미포착이며 성능·시각 개선 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-server-lod-20261101.md). 이전 기록 보존.

> 최신 진단: 기존 물리리더1파트 서버3회 완료.72 native관찰에서context+8=BulletCharacterCtrlSimple 확인. 약200ms command와CPU포즈반복 잔여. 렌더파트 물리플래그 단독은 생성 충돌. 시각/64성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-context-attachment-20261101.md). 이전 기록 보존.

> 최신 진단: 기존 서버stack을 실제mode1 평가 분기와 연결. core entity=context+0,문제의secondary=context+8은초기0으로구분. 후속 producer/타입·지원수정미확보. 새실행/PBO없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-server-owner-20261101.md). 이전 기록 보존.

> 최신 진단: Animal→CreatureAI→Creature 생성 연결. 같은3PBO 서버8파트/1파트 모두READY후충돌·포즈0.1파트fault RVA A52EE4/null read 확인,근본 원인 미확정. 기존3서명 OK. 서버 역할을 애니메이션 해결책으로 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-animal-role-20261101.md). 이전 기록 보존.

> 최신 진단: Creature 생성자에서 엔진 전역 상태에 따른mode0/1/2 선택 확인. 공개 역할 판정4개와 연결하고 기존8파트mode0 guard 재대조. mode1의 별도 평가 경로는 시각·성능 개선 증거가 아님. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-scheduler-mode-20261101.md). 이전 기록 보존.

> 최신 진단: UpdateSimulationPrecision 등록 연결·13입력/7분기 대조. 이 API가 즉시 선택하는 primary 최솟값50ms. 음수 requested0도 actual50ms. 60Hz 제어·시각·성능 수용 미확보. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-precision-contract-20261101.md). 이전 기록 보존.

> 최신 진단: 시간 필드 정적 쓰기 후보913구간/140개 분류. Head 동일오프셋과 Entity 초기66.7ms를 실제 Animal50ms에서 구분. 새 지원 수정 접점 미확보. 클라이언트/PBO 변경 없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-field-writers-20261101.md). 이전 기록 보존.

> 최신 시간 진단: 과거교대는활성관찰전부터존재. 현재결합조건조회p95 20.289ms에서도native리셋경계진행손실·CPU/화면증분연결.539HUD정합,418연속쌍.지원수정·시각·64성능미완료. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-joint-stable-phase-20261101.md). 이전기록보존.

> 최신 관찰환경 분리: 관찰기단독/녹화단독 CPU조회p95약20.2ms로과거교대패턴미재현. native평균0.833·새영상재귀2.5초로속도손실은남음. 결합조건미실행,시각/성능미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-observer-controls-20261101.md). 이전기록보존.

> 최신 애니메이션 진단: 기존4영상 위상·QPC 재분석. 학습외후보 화면증분 p95오차는현재query9.408px/이전query1.904px/native1.931px. 평균배속회복과시각부드러움수용은별개. 새게임/PBO/최적화없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-phase-matched-20261101.md). 이전기록보존.

> 최신 실시간 검증: 8개체: 8도달, 9/9낙하복구, 엄격gate True; 64개체: 64도달, 78/78낙하복구, 엄격gate True. 새pending분기 실제사용. 단일시험 기능결과이며 반복·시각·성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/entry-idle-handoff-live-20261101.md). 이전기록보존.

> 최신 후보: 등반idle 인계 단일파일9350, 서명검증·46고정조건·live엔진사전검증 통과. 기존frozen도우미 최종flag실패 보존. 실제8/64효능·시각·성능은 이체크포인트에서 미검증. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/entry-idle-handoff-candidate-20261101.md). 이전기록보존.

> 최신 실제64계측: Present631건, p95 93.070ms·CPUbusy91.088·GPUbusy12.207ms. 64/64도달·87/87낙하복구, 정렬watchdog1건으로 기존엄격기능gate실패. 시각품질 미측정·20ms미달·개선주장없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/presentation-etw-64-actual-20261101.md). 이전기록보존.

> 최신 실행자산 대조: 대표run535 retained node·8ANM 포장 일치. 기존 전체/subset 본 transform차0 trace 재확인. 새64 ETW worker는 준비·도구수명/권한거부 시험만 통과. GPU/표시/성능 미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-runtime-chain-20261101.md). 이전 기록 보존.

> 최신 대표run 대조: PSA 활성키36,736개 저장값 일치. Blender→일반/root TXA 명시채널 오차 허용범위 내. 최초 게임 추출·ANM·실제 시각품질은 미인증. 새게임 실행/PBO 변경 없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-run-source-parity-20261101.md). 이전 기록 보존.

> 최신 마감: 2026-10-31 실행에서 64/64 유효 종료·92/92 낙하 복구. script p50/p95/p99 25/58/78ms. ETW 자료 미확보, 시각·성능 목표 미수용. 2026-11-01은 종료 확인·보고일이다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/presentation-etw-64-closed-20261101.md). 이전 기록 보존.

> 최신64계측준비: 기존서명빌드/기능gate와동일시험용ETWhelper를준비했다. 이전실제CSV1495행의36분위수정합. 사용자UAC대기이며새64실행/측정/성능수용없음. [준비보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/presentation-etw-64-readiness-20261031.md). 이전기록보존.

> 최신 기존영상 대조: A/B/B/A 머리궤적의강한반복간격3.167/2.083/2.083/3.150초. 내부평균시간차이가화면에도나타남. 부드러움개선·숫자쓰기배포해결·64성능수용은아님. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-optical-carry-20261031.md). 이전기록보존.

> 최신 영상 진단: 이미지독립머리추적3종을기존영상에서대조했다. 초기2종은어깨/손가락오검출로지연평가에서기각. cap433쌍의화면/CPU이동상관은약0.982, 세임계값모두lag0최소였으나GPU지연0이나끊김해결을의미하지않음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-optical-head-20261031.md). 이전기록보존.

> 최신 애니메이션 계측: 1개체 고정run 시험에서4985프레임/29910행의6본 화면투영을 확보했다. pixel/relative는1600x900, HUD532/540 대응.12표본 공간 sanity만 확인했으며 optical궤적·시각품질·성능은미수용. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-screen-projection-20261031.md). 이전기록보존.

> 최신 애니메이션 진단: 25ms 후보는 HelicopterAuto vtable+0x880로 연결됐다. 기존영상 HUD한칸진행444쌍에서 CPU/윤곽은모두변화했고 HUD반복91쌍은몸윤곽도반복했다. 몸만정지근거없음은시각수용이나속도불균일해결이아니다. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-precision-followup-20261031.md). 이전기록보존.

> 최신 간격 setter 호출 진단: 직접 후보5곳을 대조했다. 추가 생성자 RTTI는 Helicopter/Man이며 인접25ms 분기는 Animal 지원 제어로 수용하지 않았다. 새 실행·시각/성능 수용 없음. [보고서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-precision-callers-20261031.md). 이전 기록은 보존한다.

> 최신:SupportFast표면prefix를같은입력에서두번비교했다.기하동등·분기노출은확인했으나비용은작고혼합적이어서제품에는미반영.[부정결과와제한](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/support-surface-prefix-20261031.md).실제ETW첫측정등이전기록은아래보존한다.

> 최신:ETW권한제한은이단일측정에서해소됐다.1495 Present·실제표시간격확보,표시p95 20.09ms와NPC명령진행0.654배는서로다른측정이다.최종시각/64성능수용아님.[실제결과](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/presentation-etw-baseline-20261031.md).이전권한거부는과거기록으로아래보존한다.

> 최신운영준비:PresentMon출처/서명확인,ETW권한거부로실제표시측정은없음.기존실험수치나실험시각을새측정으로바꾸지않는다.[제한과다음단계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/presentation-etw-readiness-20261031.md).이전실험은아래보존한다.

> 최신:정답시간합성에서도최근접FK위상도약58.33ms재현.이값을실제끊김시간으로직접해석하지않는다.CPU로컬버퍼모델의오차감소는공간기하진단이며품질/성능수용이아니다.[교정과한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-phase-estimator-calibration-20261031.md).이전기록은아래보존한다.

> 최신:직접입력dt연결1498/1498 f32회계일치·CPU평균진행약1배.단일진단이며최종표시/지원배포/64비용은미수용.[측정과한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-saved-dt-linked-20261031.md).실패시도도아래보존한다.

> 최신:후보saved-dt연결시도는미수용.선택조건과준비전관찰창실패를분리하고동일프로세스passive400호출을확인했다.[실패와제한](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-saved-dt-selector-20261031.md).기존결과는아래보존한다.

> 최신:기존4실행의native/CPU조회직접QPC대조.후보native진행과현재native간격일치,다음CPU조회지연은별도.최종GPU/시각/성능수용아님.[근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-native-query-order-20261031.md).이전기록은아래보존한다.

> 최신:기존6실행2482보존행의질의재사용감사.안전한cache hit·전체중복률·절감률미확립.기존반복억제표본으로중복부재를주장하지않는다.[감사범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/contact-query-reuse-audit-20261031.md).이전기록은아래보존한다.

> 최신:같은8포즈의기본/Fast32블록×3실행.구조물일부비용단축·지면추가비용·변동성보존.질의감소를64프레임개선으로간주하지않는다.[조건과원시선정](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/support-frozen-cost-20261031.md).이전기록은아래보존한다.

> 비교 조건 정정: 두 실행 모두64/64·모든낙하복귀·계약실패0이지만,반복LOW끝4개air로엄격한목표자극비교는False다.단순반복통과나후보효능/성능개선으로보고하지않는다.

> 최신64명 유한지면복구 후보:최종64/64,낙하복귀94/94,기능gate=False,여유발급0.단일실행·시각/성능미수용.[조건과근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/ground-landing-lease-repeat-20261031.md).이전기록은아래보존한다.

> 최신64명 유한지면복구 후보:최종64/64,낙하복귀90/90,기능gate=True,여유발급0.단일실행·시각/성능미수용.[조건과근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/ground-landing-lease-sixtyfour-20261031.md).이전기록은아래보존한다.

> 최신:유한지면복구서명후보·낙하세대계약반복,실제8명최종8/8·낙하복귀10/10.64/시각/성능수용은미완료.[조건과근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/ground-landing-lease-eight-20261031.md).이전기록은아래보존한다.

> 최신 작업:5번 소진 예산 차단을 기존 planner/Fall로 재현. 누계 유지·유한 지면 여유에서3포즈×2회 LAND. live FOLLOW/64회귀/시각/성능수용은 미완료. [진단과 제외한 오류](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor5-planner-fall-20261031.md). 이전 기록은 아래 보존한다.

> 최신 5번 복구 정적 재현:3포즈 착지 거부 재현,소진예산48/48 차단,새예산 진단21/48 작은이동 통과. 기존6m내1.18~1.28m 정적경로 발견. live수정/64회귀/성능수용 아님. [근거와 제외한 진단오류](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor5-landing-replay-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B93D 제한적 지지 검사64명: 최종63/64, 낙하복귀95/96, 등반종료실패0, 기능gate=False. 단일실행이며 반복성능·시각수용은별도. [조건과 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-support-fast-sixtyfour-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B93D 지지 검사 검증: shadow 적격56,648건 과소0·과대최대4.36mm,8명 활성 회귀 통과. shadow는비활성창조건으로성능대조불가.64명·반복성능·시각수용은별도미완료. [조건과 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-support-fast-eight-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B93D 64개체 고정 목표 비용 진단(2026-10-31): OFF/ON/ON/OFF p95=73.0/74.0/71.0/86.0ms. 4회 목표 검증 통과. NPC 밀침·낙하·재등반 보존. 순수 오버헤드·최적화·시각·최종 기능 수용은 별도 미해결. [조건과 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-fixed-cost-fixture-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B93D64 비용 진단(2026-10-31):OFF/ON/ON/OFF p95=96/80/64/100ms.두번째ON에서플레이어가고지대에서추락해순수계측부담/최적화수용은거부했다.발지지·관절world검사범위를관측했고전역counter64중복을배제했다.180초최종51/50/53/47명으로기능통과아님. [조건·증거·미수용이유](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-cost-instrumentation-20261031.md). 이전 기록은 아래 보존한다.

> 최신 그래프 소비 대조(2026-10-31): 기본14상태는 상태/해제/착지3변수를 소비하며 speed배속을사용하지않는다.격리TimeScale평균속도보정은과거반복시험에서확인됐지만pacing개선과전체14상태적용은미수용이다.지원되는배속제어와미확보residual/표시해결경로를구분한다.새실행없음. [근거와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-graph-consumers-20261031.md). 이전 기록은 아래 보존한다.

> 최신 공개 애니메이션 API 진단(2026-10-31): DebugOverrideAnimationSpeed의 실제 속도헬퍼는3바이트 return뿐이었다. 전역frame/slot경로도기존Animal의null시각컴포넌트가드를사용한다.새실행·배포해결·시각/성능수용은없으며전체API부재를주장하지않는다. [근거와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-public-controls-20261031.md). 이전 기록은 아래 보존한다.

> 최신 접촉 낙하 진단(2026-10-31):64명 접촉지원상실19회 모두 추적복귀,고지대목표16회 모두 후속재등반(1회는재낙하후다음세대).자세허가후약3.10초native대기가wall약4초이상으로늘어났다.새실행·수정·시각/성능수용은아니다. [근거와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/contact-recovery-chain-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B93D 보정없는 기능시험(2026-10-31): 8명:최종8/8,고지대누락[],등반실패0; 64명:최종64/64,고지대누락[],등반실패0. 각조건1회이며 반복수용·시각·성능은 미완료다. 과거B8FB실패는 보존한다. [조건과한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-free-high-20261031.md). 이전 기록은 아래 보존한다.

> 최신 B8FB 기능 대조(2026-10-31): 8/180은 최종7/8로 실패를 보존했다. 8/300은 고지대·최종8/8,64/300은 최종64/64이나 고지대52/64여서 전체 미통과다. 등반 종료/출구 계약 위반0, 정상 낙하 복귀9/9/71회를 기록했다. 새 gameplay수정이나 성능 개선을 주장하지 않는다. [조건·누락12명·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b8fb-functional-repeat-20261031.md). 이전 기록은 아래 보존한다.

> 최신 입력·표시 순서 진단(2026-10-31): 실제 saved dt합/native시간1.000029·11992개 누적산술오차0.직전script상관을잘못된엔진입력으로확정할수없다.독립영상recurrence도기본3.13초→후보2.10초/4.17초패턴을지지하지만부드러움·지원배포·8/64기능·성능은미완료다.[조건·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-input-order-20261031.md). 이전 기록은 아래 보존한다.

> 최신 역순 반복(2026-10-31): A/B/B/A·조건당2회에서 기본 clock약0.654~0.659배, 후보약1.000배로 시간 보존 효과를 재현했다. 후보 step은 직전 script interval과강하게연관되지만 인과확정·시각부드러움·지원배포·8/64회귀·성능수용은 미완료다. [조건·수치·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-residual-repeat-20261031.md). 이전 기록은 아래 보존한다.

> 최신 잔여시간 인과시험(2026-10-31): 기본 clock0.659441→후보1.000003, 독립 pose 진행0.657746→0.999111. 조건당1회·논리1명/8파트이며, 평가 횟수는약52% 증가했다. 시각 부드러움·지원 배포 경로·8/64회귀·성능 수용은 미완료다. [조건·수치·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-residual-carry-20261031.md). 이전 기록은 아래 보존한다.

> 최신 TRS·초기화 경계 진단(2026-10-31): 초기화121개를 포함해 native/CPU/영상390쌍의 시간 순서를 연결했다.397표본 모두 캐시dirty였고,123번 쌍 교체에서 끝→시작 위치/회전 차이는0이었다. 관찰부하 약4.3ms를 포함하므로 개선·최종시각·64수용은 미완료다. [조건·수치·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-trs-reset-20261031.md). 이전 기록은 아래에 보존한다.

> 최신 native/CPU/영상 부분 진단(2026-10-31): 296개 엄격 캐시 포즈 일치와144개 영상 쌍을 연결했지만,0누적기104표본/reset 경계는 일치하지 않았다.38ACK 원점 변환은 별도 검증했으며 초기 절대QPC assertion 실패를 보존했다. [조건·한계·선정 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-native-pose-join-20261031.md). 이전 기록은 아래에 보존한다.

> 최신 실제 영상 진단(2026-10-31): 883/900 HUD프레임 일치,714 CPU/화면 쌍의 변화량 상관0.943549. 클립 시간0.646964배 진행 및 주기적 큰 포즈 step 관측. 정확한 native↔영상 frame 연결·수정·64수용은 미완료다. [조건·선정 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-clip-visible-20261031.md). 이전 기록은 아래에 보존한다.

> 최신 클립 시간 진단(2026-10-31): root/dispatcher/clip kernel 각100회 연결. 클립 시간은 실제7.922초 동안4.950초 진행(0.624845배),root 증가량과 정확히 일치했다. 실제 표시·수정 효과·64비용은 미수용이다. [조건·선정 근거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-clip-consumer-20261031.md). 이전 기록은 아래에 보존한다.

> 최신 실제evaluator 시간 문맥(2026-10-31): 선택root 진입100회에서 약50ms dt와호출간격 중앙값80ms,문맥시계/host0.628을 확인했다. 최종clip/표시시간·지원제어·수정효과는 미완료다. [조건·선정이벤트·관찰기코드](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-root-consumer-20261031.md). 이전 기록은 아래에 보존한다.

> 최신 애니메이션 제어 적격성(2026-10-31): 실제1개체 읽기 관측에서 선택NativeAnimal의 generic component가 안정1839표본 모두 null이었다. 일반Animate를 스켈레톤 시계 제어로 채택할 근거는 미확보다. 새 시각/성능 개선은 주장하지 않는다. [조건·조회 함수·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/animation-generic-receiver-20261031.md). 이전 기능/접촉 결과는 아래에 보존한다.

> 최신 씬 순서 진단(2026-10-31): 컨테이너 생성 후1000ms 대기한 두 고정 접촉 시험은 각각21/21이 실제 기록과 일치했다. 이전 즉시 시험18/21은 보존하며 물리 갱신의 정확한 내부 단계는 미확인이다. 게임 이동에 대기를 추가한 것이 아니다. [조건·선정 입력·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/recovery-scene-settle-20261031.md). 아래는 이전 기록이다.

> 최신 고정4관절 재현(2026-10-31): 두 번의42행은 바이트 동일하지만 실시간 판정은18/21만 일치했다. LAND3건은 정적 컨테이너 충돌로 판정이 달라 환경 동등성은 미수용이다. [선정 입력·실패한 가설·다음 검사](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/recovery-frozen-solver-20261031.md). 이전 실제8/64 결과는 아래에 보존한다.

> 최신 실제8→64 관측(2026-10-31): 각300초 실행의 기능 결과와 같은 호출 복구 입력을 분리해 검사했다. 전체 복구 수정·시각/성능 수용은 미완료다. [결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/recovery-input-runtime-20261031.md). 아래 과거 기록은 보존한다.

> 최신 관측기 준비(2026-10-31):단일 대상 LAND/EGRESS 확장을 만들고48쌍 엔진 비교를 통과했다. 제어 시험에서 입력5행을 기록했지만 실제8/64 관측·복구 수정은 아직 미실행이다. [조건·동등성 검사·남은 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/recovery-input-observer-20261031.md). 이전 실패/충돌 재현 기록은 아래에 보존한다.

> 최신 진단(2026-10-31):61번의19개 관절 차단을 실제 엔진2회에서 반복 재현했다. 로그 시간축 차이32.18초와 LAND/EGRESS 전체4관절 입력 누락을 확인했다. 전체 복구·기능 수정은 미검증이다. [재현 범위와 누락 자료](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor61-joint-replay-20261031.md). 이전64개체의 고지대63/64 실패는 아래에 보존한다.

> 최신 계측 진단(2026-10-31): 함수별64개체 기록은 확보했지만 고지대 FOLLOW63/64로 기능 게이트 미통과.61번이 낙하/착지 복구에서 지연된 뒤 최종64/64는 도달했다. 비용 반복·시각/성능 수용도 미완료다. [조건·수치·61번 타임라인](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/follow-cost-sixtyfour-20261031.md). 이전 B93D 기능 반복은 아래에 보존한다.

> 최신 계측 진단(2026-10-31): B93D를 보존한 별도 계측 패키지에서 FOLLOW 내부 함수의8개체 실제 기록을 검증했다. 새 패키지64명·대조 반복·시각/성능 수용은 아직 미완료다. [조건·수치·실패 이력](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/follow-cost-eight-20261031.md). 아래의 이전 B93D64명 기준 측정/기능 반복은 보존한다.

> 최신 비용 진단(2026-10-31): B93D 고정 카메라·64개체 300초의 중첩 경과 타이머 기준 측정을 완료했다. 최적화 대조나 시각 품질 수용이 아니다. [측정값·조건·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/b93d-cost-baseline-20261031.md). 앞선 8/64 기능 반복 기록은 아래에 보존한다.

> 최신 반복 회귀(2026-10-31): B93D 예산 복원 ON은 8명·64명 각각 두 번 기능 게이트 통과. 64명 최종 도착 64/64 두 번, 낙하 복귀 83/81회. p95 66/60ms, 시각·성능·전체 골 미수용. [여섯 실행의 결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-six-20261031.md). 아래는 과거 보존 기록이다.

> 최신 실제8→64비교(2026-10-31): B93D의예산복원OFF/ON각300초를종료하고도착·낙하복귀와실제예산잔류표본을분리했다.후보추가반복·시각·성능수용은아직미완료다. [네실행의결과와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-four-20261031.md). 아래는과거보존기록이다.

> 최신 실제8명비교(2026-10-31): B93D의예산복원OFF/ON각300초를종료하고도착·낙하복귀와실제예산잔류표본을분리했다.64명·추가반복·시각·성능수용은아직미완료다. [두실행의결과와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-eight-20261031.md). 아래는과거보존기록이다.

> 최신 후보 패키지(2026-10-31): B93D 착지예산복원후보빌드·서명·검증성공. 실제PBO1,288행대조와실플레이씬컴파일통과. 기본OFF·원작업본보존. 새8/64·시각·성능은미수용이다. [검증범위와다음시험](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-package-20261031.md). 아래는과거보존기록이다.

> 최신 진단(2026-10-31): 동결 정체 입력에서 예산 복원8조건이 실제착지까지진행했고,정상낙하16조건은변화가없었다. 추가상태경계28행반복통과. 합성시간·동결포즈시험이며신규PBO·실제8/64·화면·성능은미수용이다. [조건과미확인범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor12-continuous-fall-20261031.md). 아래는과거보존기록이다.

> 최신 진단(2026-10-31): 12번 낙하 정체의 관절 충돌·예산 소진을 재현했고,착지 복구의1.5m 예산이 다음 낙하에 남는 상태값 누출을 확인했다. 임무전용복원후보는반복상태검사통과,새PBO·8/64·화면·성능은미검증이다. [근거와미수용범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor12-fall-budget-20261031.md). 아래는과거보존기록이다.

> 최신 실제 비교(2026-10-31): 같은 E57B 후보의8/64 OFF·ON 각300초 결과를 기록했다. 실패·후보 호출·정상 낙하 복귀를 구분하며,반복 회귀·화면 품질·성능 수용은 미완료다. [실행별 결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-runtime-20261031.md). 아래는 과거 보존 기록이다.

> 새 후보 패키지(2026-10-31): E57B 끝점 선택기 벡터 한도만 수정. 빌드·서명·검증 및 동결 대조216행 통과. 기본 작업본 보존,새8→64·시각·성능 수용은 미실행이다. [후보 범위와 검증](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-package-20261031.md). 아래는 이전 실제 시험/진단 기록이다.

> 최신 진단(2026-10-31): 60번의 −Z 표면 법선 거부를 17조건×2회 재현. 같은 호출의 raw4/이전접촉4로 실제 Accept 출력도 일치했다. 기존 선택기의 수치 한도 후보는 6dt×2회 통과했지만 게임 PBO·새8→64에는 미반영이다. [원인·후보·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor60-footprint-accept-20261031.md). 아래 실제 기능 수치는 기존 시험 기록이다.

> 최신 실제 시험(2026-10-31): 고지대 목표 유지 조건에서8/8·64/64고지대 및 최종 도착.64개체 종료 검사1건,낙하 복귀73건,p95 76ms.반복 기능·화면 품질·성능 미수용. [실행 조건과 실패 증거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/held-high-eight-sixtyfour-20261031.md). 아래 과거 수치는 원본 이력이다.

> 시험 조건 정정(2026-10-31): 기존64개체 HIGH_HOLD의 실제 플레이어 high 유지 약29.5초. 실제 high 유지 중 high+FOLLOW60/64이며, 최종64/64 도착과 별도다. 신규 런타임·수정·시각/성능 수용 아님. [원시 위치 기반 재분석](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/high-stimulus-validity-20261031.md).

> 새 애니메이션 진단(2026-10-31): idle·walk 1논리개체/8부품 고정시험에서 native 시간 진행률0.640/0.668, CPU/화면 대응 확인. 새 수정·8/64회귀·품질/성능 수용은 아님. [상세 결과](operations/idle-walk-visible-20261031.md). 아래는 이전 기능 실험 기록이다.

> 2026-10-31 코드 분석: GPUSkinning과 crowd-simulation을 고정 커밋으로 비교했다. 런타임 변경·신규 실험 없음. [적용 범위와 한계](operations/external-horde-source-review-20261031.md). 아래 실험 시각·측정값은 종전 그대로다.

# 복구 후보 실제 8→64 시험: 최종 도착과 미해결 고지대 도달 구분

> 운영 변경: Steam 최소 작업본을 기존 경로에 세팅했다. 대용량 삭제는 미완료다. 아래 실험 시각·수치는 변경하지 않았다. [최신 작업 상태](operations-latest.md)

중간 방향 복구 후보: 패키지 계약과 실제 8→64 회귀

작성: 2026-10-30T09:40:34.176533+09:00

1. 작업 목표

동결 입력에서 검증한 중간 방향 탐색을 실제 서명 패키지와8→64개체 씬에서 검사한다. 정상 낙하·복구와 벽/지지 규칙을 유지하고, 실제 기능·시각 품질·성능을 구분한다.

2. 실제 수행

기존13개 스크립트와 원래 live mission은 바이트 동일하게 유지했다. 기본OFF의FOLLOW 복구 모듈1개를 별도 아카이브로 추가했다. 기존8방향 실패,PoseClearanceActive,활성 연결 없음,진입 정렬 아님을 모두 만족할 때 중간8방향만 검사한다. 원래 PressureSlide가 실제 이동을 수행한다.

패키지 강제 OFF/ON/기본 flag 없음, 7개 상태/기존 성공 경로 대조, 벽 관통·지지 불량 음성 대조 16행을 두 번 검사했다. 기본 없음과 OFF가 행 단위 동일하고 제외 상태에서 추가 계획 0이었다. 같은 패키지 ON은 고정 입력에서 0.507013m 탈출했다. 실제 flag 파일이 있는 상태의 cold read도 추가 검사했고 강제 ON과 행 단위 동일했다. 이어 실제 클라이언트의 기존 activeReference, 300초, 동일 계측 조건을 사용했다.

3. 측정 결과

| 개체 | 설정 | 최종 도착 | HIGH_HOLD 미도달 | 낙하 후 재개 | 등반 종료 실패 | mission p50/p95/p99 ms |
|---|---|---|---|---|---|---|
| 8 | on | 8/8 | [] | 9 | 0 | 20.0/33.0/34.0 |
| 64 | on | 64/64 | [17, 33, 40] | 77 | 0 | 26.0/64.0/86.0 |

패키지 상태 대조 48행과 동결 방향 검사 288행을 세 실행에 보존했다(기본 OFF 두 반복 및 ON 파일 읽기 1회). 실제 trace의 선정 수와 실행 상태는 아래 JSON에 기록한다. p95는 진단 로그가 포함된 foreground mission dt이며 엔진 CPU/GPU 기여도나 표시 프레임 측정이 아니다. 이번 실험으로 애니메이션 개선을 판정하지 않는다.

이번 실제 ON 시험은 8/8·64/64 최종 도착이지만 64개체 HIGH_HOLD에서 17·33·40번이 미도달했다. 14번의 EGRESS_STALL 1건은 나중에 도착했으며 등반 종료 실패는 이번에 재현되지 않았다. 두 실제 실행 모두 추가 탐색 trace가 없어 실제 분기 효과는 관측하지 못했다. 이 결과를 중간 방향 수정의 성공으로 귀속하지 않는다. 이전 실패의 수리도 증명하지 못했다.

4. 원인 분석

확인된 실제 상태와 추가 계획의 표본:
json
[
  {
    "runId": "runtime-20261030-092351",
    "unresolvedTerminalAgents": [],
    "failureLabels": {
      "CONTACT_SUPPORT_LOST": 1
    },
    "algorithmFailureEpisodes": {},
    "allPhysicalTransitionsMatched": true,
    "refinementTrace": {
      "verificationStatus": "activation_unobserved",
      "sourceRunId": "runtime-20261030-092351",
      "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
      "mode": "on",
      "rows": 0,
      "actors": 0,
      "selectedRows": 0,
      "perActor": {},
      "representativeSelected": [],
      "visualAcceptance": false,
      "performanceAcceptance": false,
      "limitations": [
        "Selected goals are plan outputs,not root commits or proof of eventual arrival.",
        "128/body2048global cap;last counters are lower bounds,not final totals.",
        "No trace can mean no eligible call or unobserved IO failure;not proof of branch absence.",
        "Original retry timing includes supported-turn resets;not a guaranteed0.75s minimum interval.",
        "Logical guards are source/headless evidence;live trace directly records FOLLOW only.",
        "Plan/target/sweep counts are not CPU,GPU or wall-clock cost attribution."
      ]
    }
  },
  {
    "runId": "runtime-20261030-093001",
    "unresolvedTerminalAgents": [],
    "failureLabels": {
      "CONTACT_SUPPORT_LOST": 51,
      "EGRESS_STALL": 1
    },
    "algorithmFailureEpisodes": {
      "EGRESS_STALL": [
        {
          "runId": "runtime-20261030-093001",
          "entityId": 14,
          "eventOrdinal": 1203,
          "seconds": 87.279,
          "previousState": "EGRESS",
          "newState": "FOLLOW",
          "transitionReason": "EGRESS_STALL",
          "metadataEvent": false,
          "recoveryGenerationDerived": 1,
          "position": "<4515.811035, 339.243652, 10241.250000>",
          "contactSampleAge": 0.14399999999999125,
          "contactStatus": "PLAYER_CONTACT",
          "contactPhase": "EGRESS",
          "landingDiagnostic": "CLEAR",
          "nativeSameCallSequence": "1342",
          "nativeRecoveryGeneration": "1",
          "nativeContactStatus": "",
          "nativeMotionElapsedBeforeTransition": "0",
          "terminalOutcome": "ARRIVED"
        }
      ]
    },
    "allPhysicalTransitionsMatched": true,
    "refinementTrace": {
      "verificationStatus": "activation_unobserved",
      "sourceRunId": "runtime-20261030-093001",
      "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
      "mode": "on",
      "rows": 0,
      "actors": 0,
      "selectedRows": 0,
      "perActor": {},
      "representativeSelected": [],
      "visualAcceptance": false,
      "performanceAcceptance": false,
      "limitations": [
        "Selected goals are plan outputs,not root commits or proof of eventual arrival.",
        "128/body2048global cap;last counters are lower bounds,not final totals.",
        "No trace can mean no eligible call or unobserved IO failure;not proof of branch absence.",
        "Original retry timing includes supported-turn resets;not a guaranteed0.75s minimum interval.",
        "Logical guards are source/headless evidence;live trace directly records FOLLOW only.",
        "Plan/target/sweep counts are not CPU,GPU or wall-clock cost attribution."
      ]
    }
  }
]


선정 goal은 계획 출력이며 즉시 위치 커밋이 아니다. 계획 선정과 최종 회복의 원인 관계는 상태/연속 기록을 연결해야 한다. 128행/개체·2048행 전체 제한과 IO 미계측 때문에 trace 없음을 분기 미실행으로 단정하지 않는다. 원래 회전 복구가 재시도 시계를 다시 열 수 있어 0.75초를 엄격한 최소 간격으로 주장하지 않는다.

추정: 좁은 허용 방향에 대한 해상도 확대가 일부 접촉 복구를 돕는다. 실제 전반적 개선과 비용은 이 표본만으로 확정하지 않는다. 이전 12번 미착지·50번 등반 종료 거부는 별도 재현/수정 대상으로 보존한다.

5. 검증 수준

패키지 기본 OFF 계약은 두 반복 검증 완료, 실제 ON 파일 읽기는 1회 진단 성공. 실제 8/64는 위 실행별 결과에 한정한다. 실제 조건을 맞춘 OFF/ON 순서 교차 반복, 시각 시간 불일치 해결, 20ms p95 목표는 미수용이다. 한 실행의 64/64 또는 실패 0만으로 전체 골 완료를 선언하지 않는다.

6. 변경 사항

아카이브PBO 2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226는 빌드·서명·검증 및 14개 packed-source 일치를 확인했다. 개인 키 미포함. 작업용 D61D PBO와 운영 서버는 변경하지 않았다. compile/launcher에 명시된 별도 validation과 on/off 설정만 추가했다. 기존 씬의 자동 플레이어 이동·애니메이션 시계·접촉 규칙은 바꾸지 않았다.

7. 원시 증거

실행ID: runtime-20261030-092351, runtime-20261030-093001. 각 launch, mission snapshot, 상태 계약, 고지대 coverage, 착지/진입 분석, 접촉 입력, 추가 탐색 TSV와 패키지 검증을 보존했다. 검토 패킷은 실제 모듈 원문과 선정 계획 표본이며 전체 원시 로그는 아니다. 값이 없는 측정은 0으로 대체하지 않았다.

8. 잔여 문제

실제 미해결 사례가 있으면 해당 단계의 동일 호출 입력을 먼저 분리한다. 회복된 사례도 정상 낙하·착지·추적 재개와 목표 갱신이 이어졌는지 연결한다. 같은 패키지의 OFF/ON 조건을 순서 교차 반복하고, 추가 계획량은 빈도와 시간 범위를 나눠 측정한다. 이후 애니메이션 CPU 포즈/표시 시간 실험과 64개체 비용 기여도 실험을 별도로 진행한다. 새 관통·지지 누락·무제한 탐색·상태 계약 회귀 시 후보 수용을 보류한다.

원본 보고서 ID: 2026-10-30_094034_176533+0900_npc-refinement-runtime.md
원본 SHA-256: f8aaa758e581fa21ae5114533bd1aaeab610fb7800951f161e76a2421daad9aa
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 함수·대표 기록: [검토 패킷](../downloads/checkpoints/escape-refinement-runtime-review.txt)

[이전 기능 분석](../downloads/admission-report.txt) · [이전 애니메이션 분석](../downloads/animation-report.txt)
