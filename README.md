# DayZ NPC 현재 상태 — 2026-11-06

추가 진단: [밀집 접촉·시체 통행](reports/operations/collision-corpse-review-20261106.md) — 시체가 직접 충돌 대상으로 기록된754행 확인. 사망체의 생존용접촉솔브는정지하며정적시체충돌은남음. CPU비중은미분리.

64개체 전원 생존 표본 Present p95 **27.66ms**. ID54 제자리 회복122회와 ID17 시체지지·전리품 대기 미해결. 진단 성공, 기능·20ms 성능 목표·최종 시각 수용은 별도이며 미통과.

| 항목 | 확인 범위 |
|---|---|
|64 이동·등반|64/64 이동,60/64 한 번 이상 등반,고지대46/64 관측;수동경로|
|사망·전리품|36처치·36포즈완료·35전리품,28생존|
|중요 미해결|ID54 제자리복구122·ID17 사망접촉대기|
|전원생존 Present p50/p95/p99|15.22/27.66/35.15ms;단일30초진단|
|수집기 제한|CSV있음·exitCode null로정상종료검증실패|
|8과 비교|8의비용표본은생존7→0;통제비교아님|

[상세 보고서](reports/operations/playable-sixtyfour1-20261106.md) · [복사용 TXT](reports/operations/playable-sixtyfour1-20261106.txt) · [JSON](api/public/playable-sixtyfour1.json) · [8개체 정정](reports/errata/playable-eight2-attack-starts-20261106.md)

[AI 검토](AI_REVIEW.md) · [직전8개체 보고](reports/operations/playable-eight2-20261106.md) · [기존 작업 목록](reports/latest.md)
