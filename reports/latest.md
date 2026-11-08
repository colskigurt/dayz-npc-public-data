# DayZ NPC — 현재 상태
갱신:2026-11-08T01:20:05.975Z

**그래프변수쓰기 캐시: 실제게임플레이그래프 CPU계약 단일통과.**

- 같은그래프·타입·입력:79본/1971행,본·루트·시계차이0,요청불일치0.
- 입력고정대조는포즈가달라져그래프입력민감도확인.
- setter호출600→35회. **94.17%는호출수감소이며CPU절감률이아니다.**
- 8/64네트워크게임플레이·전체CPU·실제화면품질은미수용.명령재활성화/다른쓰기주체도미확정.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/graph-variable-write-retention-contract-20261108.md)

다음은현행5파트8개체동일조건OFF/ON통합비교.4파트후보는독립비교-10.10%/-4.40%로미수용유지.20ms p95목표미달성.

[직전후보준비](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/graph-variable-write-preflight-20261108.md)
