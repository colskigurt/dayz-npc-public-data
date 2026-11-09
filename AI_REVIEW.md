# DayZ NPC Observatory — 현재 상태

2026-11-09: **골격진단의옛기준factory실패를분리했다.** 48→248쌍시도는기준class생성중서버접근위반으로유효포즈/화면표본0. 후보·관측PBO를빼도같은RVA0xaa9859에서재현됐다. 현재64본모델/fullnative참조class는기존PBO만으로생성·명령바인딩·detached퇴역에성공했다.

- [최신 상세 보고서](reports/operations/cardinality-factory-crash-20261109.md)
- [구조화 결과](data/cardinality-factory-crash-20261109.json)
- [직전 정적입력 관문](reports/operations/cardinality-input-gate-20261109.md)
- [본매핑 해독경계](reports/operations/odol-palette-mapping-20261109.md)
- [기존 보고서](reports/operations/)

후보표시/성능실패로판정하지않는다. 정확한옛factory원인은미확정이며그복구로범위를확대하지않는다. 다음은작동한현재64본참조기반입력정정. 관측PBO서명검증성공,원본불변,모든소유프로세스종료.8/64·시각·성능·20ms목표미해결.

33ms격리비교허용답변은받았지만후보는20Hz콜백근거로이미철회됐으며구현하지않았다.50~100ms지연/다른물리오차허용으로확대하지않는다. 같은faultRVA는완전원인특정이아니며현재참조factory성공도248후보render성공이아니다.
