# DayZ 반복 상태 판정 후보 — 준비·분석

## 1. 작업 목표
Unreal 1000개체 결과를 DayZ 성과로 일반화하지 않고, DayZ 실제 반복 경로의 기존 연구와 중복을 확인한다. 사용자가 이번에는 준비·분석만 요청했으므로 새 게임·비용 시험은 진행하지 않는다.

## 2. 실제 수행
DGBNativeCrowd.Update/AccumulatePair/Movable, 지지·접촉 솔버와 기존 200개체 비용 보고서, lazy eligibility 시험 코드·계약을 읽었다. 현재 준비본의 PBO13개·미션·서버·클라이언트·수집기4개·계약/분석기2개, 총19개를 기존 SHA256과 대조해 모두 일치했다. 파일 읽기 전후 크기/mtime 안정성도 확인했다. runs 폴더는 없으며 준비본은 아직 실행되지 않았다. 게임 코드/PBO/기존 계약·분석기는 수정하지 않았다.

## 3. 측정 결과
신규 호출 횟수·CPU·Present·서버 dt 측정 없음. 과거 독립 유효2쌍은 전체 서버 CPU 평균 625.4568→594.0901(-5.0150%), 612.0723→602.6553(-1.5385%) ms/벽시계초였다. 세 번째 쌍은 전면 창 조건 위반으로 제외됐고 이번 점검으로 복권하지 않는다. 유효2쌍 중앙 약3.28% 감소는 최종 개선율 또는 채택 기준 통과가 아니다. 19/19 입력 무결성 대조는 성능·기능 시험이 아니다.

## 4. 원인 분석
확인된 반복 후보는 AccumulatePair의 Movable(body) && !EntryPressurePriority다. 기존 공간 격자가 만든 인접 개체 쌍마다 같은 개체의 상태 필드를 반복 조회한다. 후보는 같은 Crowd.Update 세대 안에서 개체가 처음 필요할 때 값을 계산해 재사용한다. 다음 Update에서 세대를 갱신하고 재진입·OpenTraversal 비활성에는 기존 경로로 돌아간다. 이 세대는 군집 갱신 범위 표식이며 개체 lifetime generation이나 월드 revision의 대체물이 아니다.
현행 쌍 누적 경로는 상태 판정 후 접촉 힘을 누적하며 그 사이 Movable 의존 상태를 변경하는 사용자 콜백은 소스에서 발견하지 못했다. 임의 확장 코드나 비동기 변경까지 안전하다는 일반 보장은 아니다. 다른 코드가 상태·목록을 변경하도록 확장되면 이 재사용 범위를 재검증해야 한다.
물리 Ray/Sphere·바닥 지지 결과·포즈·경로는 캐시하거나 생략하지 않는다. Drive와 Displace는 anchor/heading/lift·관측 시점이 달라 동일 결과라고 볼 근거가 없으므로 솔버 결과 공유 후보를 새로 만들지 않았다. Unity CaptureRevision/MeasureActionRoot/QueryTargets의 비용을 DayZ 수치로 옮기지 않았다. 주요 전체 물리 비용을 이 작은 상태 조회 캐시가 해결한다고 주장하지 않는다.

## 5. 검증 수준
기존 연구 재사용·사전 무결성 점검 완료. 수정 구현은 기존 격리 미션에 이미 존재하며 이번 새 구현은 없다. 세 번째 유효 비교·기능 최종 수용·화면 품질·Present·64/1000개체 확장 수용은 미완료다. 과거 두 유효 비교에서도 terminal 결과가 달랐으므로 완전 행동 동등성으로 표현하지 않는다.

## 6. 변경 사항
source-review-preparation-20261118.json과 선택 함수 검토 패킷, 이 체크포인트만 추가했다. 기존 시험·운영·서명 PBO·미션·원본 모델은 보존했다. PBO 변경이 없으므로 신규 빌드/서명 대상 없음. 기존 서명 성공 이력을 이번 재서명·서명 도구 재검증으로 보고하지 않는다.

## 7. 원시 증거
기존 research/rate100-isolation-20261112/lazy-eligibility-cost64-replacement1의 contract.json, preparation-proof.json, prelaunch-audit-20261114.json, mission/init.c, run-pair.ps1, analyze-pair.py를 재사용한다. 새 audit은19개 경로/기존 SHA/현재 SHA/일치·안정성을 연결한다. repeat-review-evidence-20261118.txt에는 baseline Movable/쌍 누적 및 후보1~37행을 선택 발췌했다. 전체 코드나 새 원시 실행 자료로 표현하지 않는다. 과거 유효/제외 판정은 2026-11-12_214206_126575+0900_lazy-eligibility-repeat-condition-exclusion.md 및 이후 관측 수 정정 기록을 유지한다.

## 8. 잔여 문제
다음 단일 산출물은 기존 동일64개체 baseline→ON의 세 번째 유효 비용 비교 결과다. 사용자 전용 foreground 시간 허가 후에만 기존 run-pair를 사용한다. 고정 미션·PBO13개·64render/64receiver·40게임초 워밍업·30벽시계초 CPU·같은 진단을 유지한다. 오류·크래시·비활성·host gate 실패면 비교를 제외하고 다음 조건으로 자동 확대하지 않는다. 원래 세 쌍 모두 CPU 감소·중앙5% 기준을 유지하며 미달이면 미수용으로 종료한다. 낙하/착지/밀침·재등반 및 terminal 차이를 독립 판정하고 기준 공통 결함 때문에 기능 개발로 선회하지 않는다. 미션 dt와 CPU를 Present/RTT로 환산하지 않는다. 현재는 사용자의 준비·분석 제한으로 실행 보류다.
