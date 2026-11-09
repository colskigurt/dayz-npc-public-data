# DayZ NPC Observatory — 현재 상태

2026-11-09: 64개체 선택 접촉 차단 후보는 **미수용**이다. 서버 CPU 관찰값은 759.364→839.592 CPU ms/벽시계초(+10.565%)였지만 클라이언트 script dt 중앙값이 259→20ms로 달라 통제된 인과 비교로 인정하지 않는다. 실제 Present와 화면 품질은 미측정이다. 64체 로컬 블록 생성/해제 회계는 확인했고 기존 PBO는 변경하지 않았다.

- [최신 상세 보고서](reports/operations/selective-peer-block-cost64-screen-20261109.md)
- [최신 구조화 데이터](data/selective-peer-block-cost64-screen-20261109.json)
- [앞선 8개체 기능 검증](reports/operations/selective-peer-block-eight-20261109.md)
- [기존 보고서 전체](reports/operations/)

다음: 기존 진행률 자료로 접근 감속 적용/금지 조건을 정리한다. 지속 등반·정상 밀침·낙하·재등반을 보존하며, 이 후보의 장시간 회귀나 유리한 재실행은 진행하지 않는다. 20ms p95는 여전히 목표다.

외부 검토 시 단일 쌍·클라이언트 갱신 조건 불일치·실제 표시 미측정을 반드시 유지해 해석한다. CPU 관찰 증가를 후보의 확정 회귀율로 보지 않는다.
