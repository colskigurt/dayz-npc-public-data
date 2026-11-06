# DayZ NPC 현재 상태

새 게임0. 같은64개체의 초기/밀집구간 격자 후보 평균636/992. 스냅샷 재구성이며 실제 callback 계수·CPU 증가율 아님. 격자 재사용은 타당성 검토,미구현.

| 항목 | 상태 |
|---|---|
| 이번 작업 | 기존 로그 오프라인 진단 |
| 최근 실제64 비용 후보 | receiver 방향 생략: Present p95 OFF19.49/ON19.81ms,단일쌍 미수용 |
| 평지 기준 | 3회 p95 14.55~18.82ms,별도 조건 |
| 전체 목표 | 밀집/고저차/전투 성능·기능·시각 각각 미완료 |

[상세](reports/operations/grid-density-existing-evidence-20261107.md) · [근거 TXT](evidence/grid-density-existing-evidence-20261107.txt) · [JSON](api/public/grid-density-existing-evidence.json)

[최근 실제 비용시험](reports/operations/receiver-orientation-cost64-pilot-20261107.md) · [AI 검토](AI_REVIEW.md)
