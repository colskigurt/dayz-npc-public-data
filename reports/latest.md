# 최근 실측과 현재 준비 상태

최근 실측은 정지Run3쌍에서5파트 CPU Busy/Present p95 감소 방향 재현. 전체 게임/시각 미수용. [실측](operations/npc-palette64-client-highcap-three-pairs-20261105.md)

## 이후 준비 — 신규 측정 없음

14클립을 한 서버에서 순차 대조하는 소스 준비. 5쌍·401본×4벡터와 루트/콜백 시간 요약. 컴파일·실행·성능 측정 없음. Native65클립은 편집기 파일 접근 승인 대기. 실제 상태 전환·이벤트·시각 검증은 별도.

[시험 준비](operations/npc-palette64-fullclip-pose-fixture-staged-20261105.md)

## 2026-11-06: 4파트 전체클립 입력 준비

4파트 후보의 전체14클립 입력 계약 확인. 신규52TXA 준비·Run4재사용. 이번 묶음의 네이티브 변환/게임 실행은0회이며 전체14클립 포즈·8/64기능·비용·화면 품질은 미검증이다. [상세](operations/four-part-fullclip-inputs-20261106.md). 위의 기존 측정값·실험 ID·반복 수는 변경하지 않았다.

## 2026-11-06: 4파트14클립 CPU 대조

4파트 전체14클립 네이티브 변환·CPU 포즈 대조를 단일 서버 실행에서 통과했다. 331본의 정렬 구간 변환·루트 차이0. 게임플레이·시각·성능 수용은 별도 미검증이다. [상세](operations/four-part-fullclip-native-pose-20261106.md). 이번 측정에는 Present/CPU 비용 값이 없다.

## 2026-11-06: 4/5파트8명 기능 대조

4파트 후보와5파트 대조군 모두8/8 전체 이력·최종 도달,낙하9/9복귀·상태 계약 오류0. 각1회이며64명·시각·전투·CPU/Present 비용은 미검증이다. [상세](operations/four-part-eight-functional-20261106.md). 기존 비용 측정값은 이번 기능 시험 결과가 아니다.

## 2026-11-06: 4파트64명 기능 회귀

4파트64명 단일 기능 시험에서64/64 전체 이력·최종 도달,낙하93/93 LAND/FOLLOW복귀·상태 계약 오류0. 성능·시각·전투는 미검증이다. [상세](operations/four-part-sixtyfour-functional-20261106.md). 새 CPU/Present 수치는 없다.

## 2026-11-06:4파트 비용3쌍 완료

4파트64명 기능은 단일64/64 통과. 새5대4파트 비용6회/3쌍은 방향 불일치로 채택 실패.5파트 기준 유지. 시각/전투/전체20ms는 미수용. [상세](operations/four-part-cost-three-pairs-20261106.md).

## 2026-11-06:서버 포즈 소비자 검토

5파트 기준 유지. 4파트 비용3쌍은 방향 불일치로 미수용. 이번은15개 핀 PBO의 서버 포즈 소비자 정적 검토이며 새 성능값·게임 실행은 없다.  [상세](operations/server-pose-consumers-20261106.md).

## 2026-11-06:비대표 루트 조회 분기 시험

5파트 기준 유지. 격리 루트 조회 후보 빌드·서명·3모드 분기 시험 완료. 진단 콜백19회로 커버리지 부족,실제8/64·CPU·시각 미검증. [상세](operations/follower-root-branch-20261106.md).

## 2026-11-06:비대표 루트 후보8명 회귀

5파트 기준 유지. 비대표 루트 조회 생략 OFF/ON 각8/8 통과,낙하9/9·10/10복귀·상태 계약 오류0. 비용·시각·64명 미검증. [상세](operations/follower-root-eight-20261106.md).

## 2026-11-06:루트 조회 국소 비용3쌍

5파트 기준 유지. 비대표 루트 조회 후보는8명 기능 통과했지만 국소 비용3쌍의방향이불일치해미수용.64명 확대중단. [상세](operations/follower-root-method-cost-20261106.md).

## 2026-11-06:현재5파트비용근거범위

기존5파트LOW고정/평지의반복client p95는20ms아래. 전체고지대경로는64/64기능과serverdt근거가있으나같은실행의client/native비용귀속은없음. 새실행0회. [상세](operations/current-five-cost-coverage-20261106.md).

## 2026-11-06:현재5파트HIGH목표진단

검증된5파트 유지. 새HIGH목표단일진단의Present p95는14.98ms였지만HIGH다수관측조건실패로미수용. 별도서버CPUquery union24.85%inclusive,스택누락26.17%. 새최적화없음. [상세](operations/high-five-diagnostic-20261106.md).

## 2026-11-06:상태제한SupportFast5파트준비

상태제한SupportFast는이미구현·시험된후보다. 현재5파트메서드동일확인,legacy의존성만분리한동일script어댑터빌드·서명검증.8명OFF/ON계획준비,아직미실행·비용미수용. [상세](operations/support-context-five-preparation-20261106.md).

## 2026-11-06:상태제한SupportFast 현행8명 기능

상태제한SupportFast 현행5파트 OFF/ON 각1회8/8·낙하9/9복귀·계약오류0. ON全8개체사용확인. 비용·시각·64명최종미수용.  [상세](operations/support-context-five-eight-20261106.md).

## 2026-11-06:상태제한SupportFast 현행8명 기능

현행64명 SupportFast ON 첫전체CPU=864.92 CPUms/벽시계초,게임시계약1배. 기준CPU는날짜정밀도변환으로누락·수정. 유효비교0쌍,성능미수용.  [상세](operations/support-context-five-cost-first-20261106.md).
