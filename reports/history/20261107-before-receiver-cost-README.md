# DayZ NPC 현재 상태

수동64와평지64의solve율은약39/40회·개체게임초로유사. 전투receiver64개추가구성차이확인. 새8ON은동일방향setter3478회(19.38%)생략·최종방향불일치0,전체비용미측정.

| 항목 | 판정 |
|---|---|
| 최근평지64 기준 |3회 Present p95 14.55~18.82ms,새최적화아님 |
| 새8국소후보 |동일값setter생략만검증,피격geometry/64비용미검증 |
| shadow 종료 |퇴역재진입null예외,ON단일run에서미재현 |
| 기존시각 |사용자14클립/8개체평가보존,이번미측정 |
| 전체목표 |밀집/고저차/전투비용·전체20ms 미수용 |

[상세](reports/operations/receiver-orientation-eight-20261107.md) · [근거 TXT](evidence/receiver-orientation-eight-20261107.txt) · [JSON](api/public/receiver-orientation-eight.json)

[AI검토](AI_REVIEW.md) · [평지기준](reports/operations/five-flat-baseline-three-20261107.md)
