# DayZ 커스텀 NPC — 현재 상태와 연구 근거

기준: 2026-11-03. **64개체 CPU·물리 비용 절감이 주력이며, 최종 수용과20ms p95 목표는 미달성입니다.**

| 확인 대상 | 확인된 결과 | 조건·검증 수준 |
|---|---|---|
| 8개체 기능 | 최근 방향 후보8/8 | 고정 등반·하강·재등반 시나리오1회; 최종 반복 수용 아님 |
| 64개체 고지대·전체 이력 | 53/64 완료 | 같은 후보1회. 나머지11명은 종료 시 FOLLOW였지만 고지대/하강/재등반 이력 미충족; 모두 정체라는 뜻은 아님 |
| 해당64개체 낙하 복귀 | 82/82 LAND→FOLLOW | 정상 낙하를 허용. 이것만으로 전체 기능 통과 아님 |
| 전체 프레임 비교 | 커스텀 fresh65.47–69.56ms / continued70.09–75.30ms; 바닐라fresh24.23–24.44ms | 과거 동일 평지 비교4회. 현재 네트워크 후보의 성능값으로 전용하지 않음 |
| 목표 대비 격차 | fresh약3.3–3.5배 / continued약3.5–3.8배 | 목표20ms p95. 서버 dt와 Present를 섞은64–241ms 범위는 사용하지 않음 |
| 애니메이션 | 네트워크 경로 개선 근거·사용자 개선 관찰 있음 | 전체14클립/8파트/64개체의 시각·성능 최종 수용은 미완료; 과거 mode0의0.65배를 현재 전체 경로로 일반화하지 않음 |
| 현재 물리 비용 후보 | 지형 API 국소130행, 적격110행·분류차이0·최대 높이차0.031mm | 단일 진단만 통과. 비용·8/64기능·시각 품질은 미검증 |

현재 기능 수치의 기준은 `entry-facing-sixtyfour-on1`이며 성능 비교는 별도 `matched-flat-pursuit-20261102/detailed`입니다. 서로 다른 빌드/실행을 하나의 성능·기능 성공으로 합치지 않습니다.

## 지금 진행할 작업

기존10ray와 `비지형 Overlap + 지형 조회`의 국소 비용을 비교합니다. 두 순서에서5% 이상 이득·비적격 경로3% 이내 증가 기준을 못 넘으면 군집 시험으로 확대하지 않습니다. 등반 기능 복구 후보와 단순8→1호스트 통합은 보존·보류합니다.

## 근거와 외부 검토

- [최신 작업 기록](reports/operations-latest.md) · [간결한 AI 검토 안내](AI_REVIEW.md) · [현재 상태 JSON](api/public/current-status.json)
- [고정 검증·채택 기준](reports/operations/npc-validation-policy-20261103.md) · [이번 외부 검토에 대한 답변](reports/operations/npc-external-review-response-20261103.md)
- [64개체 기능 판정](reports/operations/npc-entry-facing-regression-20261103.md) · [평지 비용 비교](reports/operations/matched-flat-pursuit-20261102.md)
- [지형 API 진단](reports/operations/npc-terrain-support-contract-20261103.md) · [8파트 통합 보류](reports/operations/npc-host-reduction-decision-20261103.md)
- [선정 핵심 코드·판정 문맥](evidence/npc-review-source-context-20261103.txt) · [선정 원시 비용행·종료 판정·8/64설정](evidence/npc-review-cost-and-scenarios-20261103.json)

## 과거 기록

[이전 README 누적 안내](reports/history/20261103-before-external-review-README.md) · [이전 AI_REVIEW 누적 안내](reports/history/20261103-before-external-review-AI_REVIEW.md) · [날짜별 보고서](reports/operations/)

사람용/AI용 진입 문서는 현재 상태를 교체 갱신하고 과거 요약을 계속 앞에 붙이지 않습니다. 원시 증거와 날짜별 보고서는 보존합니다. 공개 인터페이스는 읽기 전용이며 게임 자산·키·내부 원시 전체를 공개하지 않습니다.
