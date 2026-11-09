# DayZ NPC Observatory — 현재 상태

2026-11-09: **33ms 밀침 묶음 후보를 구현 전에 철회.** 기존 최신64개체 모두 물리 콜백 평균19.99698Hz(약50ms)여서33ms 조건이 호출량을 줄인다는 근거가 없다. 앞선 선택 질문은 철회됐다. 더 긴 지연으로 자동 변경하지 않는다.

- [최신 상세 보고서](reports/operations/contact-cadence-feasibility-20261109.md)
- [구조화 결과](data/contact-cadence-feasibility-20261109.json)
- [실제64 군중 범위 관측](reports/operations/crowd-live-attribution-20261109.md)
- [기존 보고서](reports/operations/)

같은 표본 전체Solve121,908회 중Displace50,833회(41.70%)는 호출량 비율이며CPU비중이 아니다. 물리콜백20Hz를전체애니메이션/렌더20fps로해석하지 않는다. 새게임·구현·PBO없음. 현재즉시적용할새안전후보는미확정. 전체CPU·시각품질·20ms p95 목표는미해결.

1mm 발지지 허용은 다른후보나일반밀침오차허용으로확장하지않는다. 캐시입력불일치·중간Contacts·부분성공rollback을보존해야한다. 현재노력은성능개선성공이아닌구현전오류전제소거다.
