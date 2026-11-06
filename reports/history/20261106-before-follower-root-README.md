# DayZ NPC 현재 상태

5파트 기준 유지. 4파트 비용3쌍은 방향 불일치로 미수용. 이번은15개 핀 PBO의 서버 포즈 소비자 정적 검토이며 새 성능값·게임 실행은 없다. 

| 항목 | 판정 |
|---|---|
| 4파트64명 기능 | 단일64/64·낙하93/93복귀 |
| 5→4 비용3쌍 | Present p95−26.8/+6.1/−11.0%,미수용 |
| 서버 포즈 소유권 | 대표 본 소비 확인,비대표 native 의존성 미확인 |
| 다음 후보 | 비대표 루트 getter/누적만 opt-in 생략,미구현 |
| 시각·전체20ms | 최종 미수용 |

[상세](reports/operations/server-pose-consumers-20261106.md) · [TXT](reports/operations/server-pose-consumers-20261106.txt) · [근거](evidence/server-pose-consumers-20261106.txt) · [JSON](api/public/server-pose-consumers.json)

[비용 보고서](reports/operations/four-part-cost-three-pairs-20261106.md) · [AI 안내](AI_REVIEW.md) · [직전 안내](reports/history/20261106-before-server-pose-README.md)
