# DayZ NPC Observatory — 현재 상태

2026-11-09: **스키닝 셰이더의 정적 버퍼는255행렬.** 실제설치system.pbo의 $SKIN vertex shader118/118에서같은선언을확인했다. 따라서64 weighted본예산을shader확정용량으로설명할수없다. 실제모델팔레트구축/업로드/본index매핑한계는미확인이다.

- [최신 상세 보고서](reports/operations/skin-shader-capacity-20261109.md)
- [구조화 결과](data/skin-shader-capacity-20261109.json)
- [철회한33ms 접촉후보](reports/operations/contact-cadence-feasibility-20261109.md)
- [실제64 군중범위관측](reports/operations/crowd-live-attribution-20261109.md)
- [기존 보고서](reports/operations/)

기존단일전체/텍스처구간분할의시각실패는유지한다.1파트가능성확정아님. 기존모델도이미ODOLv56이므로단순binarization누락이아니다. 새게임/후킹/자산변경/PBO없음. 전체CPU·시각·20ms p95목표미해결. 다음은기존컴파일ODOL의팔레트/index비교다.

공식1.30의250global bone제한제거는per-model skin palette와동일개념이아니다.4파트산술하한은64예산을가정한조건부정적결과다. 실제draw관측없이GPU업로드/단일host지원/성능효과를선언하지않는다. 원시shader와전체해독문은공개하지않는다.
