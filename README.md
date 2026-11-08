# DayZ NPC — 현재 상태
갱신:2026-11-08T01:49:56.895Z

**64 CPU효과 미확정·ON교체측정대기.새게임실행없음.**

기존바이너리부분검토에서PreAnim_SetInt/Bool등록4개본문은6/8명령값저장으로끝나며그래프평가/물리/할당호출을포함하지않았다.그러나VM경계전체비용은미확정이고상위100함수요약의부재를0비용으로해석하지않는다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/preanim-setter-native-cost-boundary-20261108.md)

유효OFF서버CPU852.45ms/벽시계초.ON은창비활성화로무효여서비교값/개선률없음.창유지가능답변전재실행하지않는다.전체기능·시각·성능미수용,20ms p95목표미달성.

[계측제거계약과무효측정](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/graph-variable-write-clean-cost64-invalid-20261108.md)
