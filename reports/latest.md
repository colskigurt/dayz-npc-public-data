# DayZ NPC — 현재 상태
갱신:2026-11-08T01:02:05.791Z

**4파트 최적화 후보: 효과 미확정·미수용.**

|독립비교|5파트 CPU|4파트 CPU|감소|5%선별|
|---|---:|---:|---:|---|
|첫쌍5→4|806.62|725.16|10.10%|통과|
|역순4→5|788.22|753.50|4.40%|미통과|

단위는CPU ms/벽시계초이며프레임시간이아니다.두번째결과가사전기준을넘지못했으므로첫결과나평균만으로채택하지않는다.효과부재를입증한것도아니다.동적접촉·등반·낙하작업량이다르다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-cost64-reverse-repeat-20261108.md)

기능·시각·최종성능미수용.4파트8개체EGRESS위험미확정.실제Present/GPU미측정,20ms p95목표미달성.추가반복을확대하지않고후순위보존한다.

[첫쌍 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-cost64-first-screen-20261108.md) · [8개체 기능](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-eight-mixed-20261108.md)

다음:기존비용증거를재사용해아직미검증인구조적후보한개를선정.기능개발로주력을전환하지않는다.
