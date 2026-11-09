# DayZ NPC Observatory — 현재 상태

2026-11-09: **접근 감속은 구현 보류**. 기존 분류기를 저장된64체 기준 로그에 재사용했다. 전방 정체+반대 압력 구간은 관측 Displace의10.174%, 명목 등반 입구2m 밖까지 제한하면0.898%였다. 이는 CPU 절감률이나 제거 가능량이 아니다. 현재 native root delta 분기와 별도 접촉 소비 구조에서는 speed 입력만 줄여도 SolveContacts가 계속 호출된다.

- [최신 접근 감속 검토·적용 금지 조건](reports/operations/approach-slowdown-readonly-review-20261109.md)
- [최신 구조화 데이터](data/approach-slowdown-readonly-review-20261109.json)
- [직전64체 CPU 비교의 조건 불일치](reports/operations/selective-peer-block-cost64-screen-20261109.md)
- [앞선8체 이동 검증](reports/operations/selective-peer-block-eight-20261109.md)
- [보고서 목록](reports/operations/)

새 게임 실행0, 원본/PBO 변경0. 지속 등반·밀침·낙하·재등반 유지. 기능·시각·성능을 분리하며20ms p95는 목표다. 다음은 기존 기각 후보를 재실행하지 않고, 질의/호스트 비용을 줄일 미검증 국소 후보의 유무를 판단한다.
