# DayZ NPC 현재 비용·검증 상태

검증된5파트 유지. 새HIGH목표단일진단의Present p95는14.98ms였지만HIGH다수관측조건실패로미수용. 별도서버CPUquery union24.85%inclusive,스택누락26.17%. 새최적화없음.

| 근거 | 판정 |
|---|---|
| 기존5파트LOW/평지 | 각각3회client p95<20ms·조건한정 |
| 새HIGH목표진단 | p95 14.98ms·HIGH다수조건실패 |
| 별도서버CPU | 질의경계24.85%inclusive·self-time아님 |
| 전체경로·시각·전투·20ms | 최종미수용 |

[상세](reports/operations/high-five-diagnostic-20261106.md) · [TXT](reports/operations/high-five-diagnostic-20261106.txt) · [근거](evidence/high-five-diagnostic-20261106.txt) · [JSON](api/public/high-five-diagnostic.json)

[이전근거범위](reports/operations/current-five-cost-coverage-20261106.md) · [AI 안내](AI_REVIEW.md) · [직전 안내](reports/history/20261106-before-high-five-README.md)
