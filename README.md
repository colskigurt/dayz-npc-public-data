> **현재 작업 방향:** [실전384호스트 혼합장면과평지320호스트의비용범위정리](reports/operations/mixed-gameplay-cost-decision-scope-20261108.md).다음은현행엔진의통제된혼합장면기준선준비이며,신규성능개선은아직미검증입니다.

> **최신 최적화 시험:** 표시 호스트 solid64 후보는 서버CPU−3.03%지만 Present p95+14.23%로 **선별 미통과·미수용**입니다. [비교 수치·근거·다음 작업](reports/operations/visual-solid-flat64-screen-result-20261108.md).

> **시험 조건 정정 (2026-11-07):** 앞선 평지 64개체 접근 선별 시험은 전투 OFF·320개 표시 호스트 조건입니다. 피해 수신 호스트를 포함했다는 설명을 정정합니다. [진단과 정정 근거](reports/operations/host-physics-cohort-correction-20261107.md). 성능 수치와 후보 미수용 판정은 유지합니다.

# DayZ NPC 연구 현황

> 보조 진단(2026-11-07): [회전 산술의 국소 효과와 전체 기여 평가](reports/operations/rotation-math-method-weighting-20261107.md). 로컬 효과 확인·전체 절감 미입증으로 후순위,새 PBO 없음.

> 최신 후보 판정(2026-11-07): [접근 조기탈락64 비용 선별 실패](reports/operations/approach-prereject-flat64-screen-20261107.md) · [JSON](data/approach-prereject-flat64-screen-20261107.json). 서버CPU +5.09%,Present p95 +6.11% 단일쌍; 미수용·반복확대하지 않음.

> 진행 중(2026-11-07): [검사 순서 변경 후보 준비](reports/operations/approach-prereject-preparation-20261107.md). 기본OFF·오프라인 계약만 확인, 실제 성능 미검증.

> 구조 분석(2026-11-07): [Days Gone 충돌·지지와 DayZ 적용 단서](reports/operations/days-gone-contact-support-clues-20261107.md) · [근거 JSON](data/days-gone-contact-support-clues-20261107.json). 기존 연구 재검토이며 새 성능 개선 판정은 없습니다.

> 참고 진단(2026-11-07): [Days Gone 지붕 호드 비용·표시 계측](reports/operations/days-gone-roof-cost-20261107.md) · [JSON](data/days-gone-roof-cost-20261107.json). 단일 유효 수집이며 DayZ 최적화 성과는 아닙니다.

최신: 2026-11-07 — 64개체 내부 계측 제거 후보 비용 비교

| 항목 | 현재 판정 |
|---|---|
| 비용 시험 | 유효 캡처3건, 완료1쌍/계획3쌍 |
| 첫쌍 서버CPU | 912.238→959.076ms/게임초, +5.13% |
| 첫쌍 Present p95 | 18.313→15.365ms, -16.10% |
| 후보 채택 | 미수용. 작업량 차이·CPU 감소gate 실패·반복 미완료 |
| 중단 이유 | 4번 대조군 native crash, 나머지2회 미실행 |
| 전체 기능·시각·20ms 목표 | 이번 시험으로 수용하지 않음 |

[최신 상세 보고서](reports/operations/contact-timing64-cost-interim-20261107.md) · [복사용 텍스트](reports/operations/contact-timing64-cost-interim-20261107.txt) · [공개 JSON](api/public/contact-timing64-cost-interim-20261107.json)

LOW_HOLD 고정 목표·64논리/384native hosts·공격OFF·1280×800/VSync0·각15초 조건이다. 서버CPU는 스레드 합산 프로세스량이며 프레임 시간이 아니다. 후보3번은 미완료한2번째 쌍이므로1번과 재짝짓기하지 않는다. 원시 기록과 과거 보고서는 보존한다.

[외부 AI 검토 안내](AI_REVIEW.md) · [보존된 운영 보고서](reports/operations/) · [과거 첫 화면](reports/archive/99e8ed18/README.md)

참고 조사: [Days Gone 접촉·지지·밀침·애니메이션 기존 증거](reports/operations/days-gone-existing-contact-review-20261107.md). 새 성능 실험이 아니며 위 최신 비용 판정은 유지한다.
