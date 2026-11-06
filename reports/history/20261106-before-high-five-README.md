# DayZ NPC 현재 비용·검증 범위

기존5파트LOW고정/평지의반복client p95는20ms아래. 전체고지대경로는64/64기능과serverdt근거가있으나같은실행의client/native비용귀속은없음. 새실행0회.

| 근거 | 현재 판정 |
|---|---|
| LOW고정64·5파트 | 기존3회client p95 12.544~14.083ms |
| 평지추적64·5파트 | 기존3회client p95 14.506~16.887ms |
| 전체고지대64 | 단일64/64·낙하89/89,client비용미확보 |
| FOLLOW접촉solve | 호출량95.96%,CPU비중아님 |
| 루트getter후보 | 기능통과·국소비용실패,64확대없음 |
| 전체20ms·시각·전투 | 최종미수용 |

[상세](reports/operations/current-five-cost-coverage-20261106.md) · [TXT](reports/operations/current-five-cost-coverage-20261106.txt) · [근거](evidence/current-five-cost-coverage-20261106.txt) · [JSON](api/public/current-five-cost-coverage.json)

[AI 안내](AI_REVIEW.md) · [직전 안내](reports/history/20261106-before-cost-coverage-README.md)
