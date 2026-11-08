# DayZ NPC — 현재 상태
갱신:2026-11-08T00:53:40.853Z

|항목|5파트|4파트|
|---|---:|---:|
|서버 CPU ms/벽시계초|806.62|725.16|
|관측 생존·인식·2m이동|64/64|64/64|
|접촉처리율 /trace게임초|2550.69|2461.35|

**CPU -10.10%는 단일쌍 선별 결과이며 최적화 최종 성공이 아니다.** 접촉작업률도3.50%낮고동적경로가달라순수인과효과미확정. 역순독립반복이필요하다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-cost64-first-screen-20261108.md)

미션dt p95는44.7→38ms지만Present가아니다. 실제화면품질·Present/GPU미측정.전체기능미수용,4파트8개체EGRESS위험미확정.20ms p95는목표·미달성.

[8개체 기능 비교](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-five-eight-mixed-20261108.md) · [접촉 경계 검토](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/current-four-contact-boundary-20261108.md)
