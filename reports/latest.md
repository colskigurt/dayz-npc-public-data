# DayZ NPC — 현재 상태

2026-11-09:64개체 경로 접선·접촉 변위 관측1회. 감속은 미적용입니다.

|항목|관측 결과|
|---|---|
|Displace 시도/성공/거부|64,234 / 62,275 / 1,959|
|내부 솔버/전체 솔버|63,262 / 162,076 — CPU 비율 아님|
|밀집 request1 경로 진행|1.363m/s, Displace19.89회/개체·초|
|비밀집 request1 경로 진행|2.238m/s, Displace8.49회/개체·초|
|저진행 이동 구간|이동시간7.64%, 전체Displace2.53%|
|종착/전체 기능|12/64 직접종착, 엄격 분석 milestone coverage 불충족 — 미수용|
|CPU·시각 개선|미측정|

밀집 이동은 대부분 이미request1입니다. 단순 달리기→걷기 또는일괄감속은 적용하지 않습니다. 후열 압력 완화는 미검증 후보입니다. 지속 등반·밀침·낙하·재등반 정책은 변경하지 않았습니다.

[상세 보고서](operations/ground-progress-observation-sixtyfour-20261109.md) · [공개 수치](../data/ground-progress-observation-sixtyfour-20261109.json)

[8개체 관측](operations/ground-progress-observation-eight-20261109.md)과 [기존 로그 분석](operations/ground-progress-existing-20261108.md)을 보존합니다. 기능·시각·성능은 별개이며20ms p95는 목표입니다.
