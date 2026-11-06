# DayZ NPC 현재 상태

현재 통합 계측기의 종료코드 누락 대응3조건 회귀 통과. 새 ETW/NPC 성능 측정 없음. 기존 단순 결합솔버 기각과 실제8 Bounds hit0 보류 유지.

| 항목 | 판정 |
|---|---|
| 기존 애니메이션 |사용자14클립·8개체평가 성공,64최종품질 별도 |
| 최근8 shadow |8/8·낙하8회 복귀,cache hit0으로 주력후보 보류 |
| 최근64 수동 비용 |전원생존30초 Present p95 27.66ms,단일실행·수집기종료 미수용 |
| 이번 계측기 |0/7/CSV없음 분기 통과,실제ETW 미실행 |
| 목표 |64개체 p95 20ms·미달성 |

[상세](reports/operations/collector-exit-contract-20261107.md) · [근거 TXT](evidence/collector-exit-contract-20261107.txt) · [JSON](api/public/collector-exit-contract.json)

[최신 게임시험](reports/operations/bounds-update-eight-shadow-20261107.md) · [AI검토](AI_REVIEW.md)
