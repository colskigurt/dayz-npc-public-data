# DayZ NPC Observatory — 현재 상태

2026-11-09: **ODOL구간별본매핑을원본과부분대조.** 기존5모델의모든LOD를읽었다. 전체모델하위본표2160항목,텍스처분할445,64본part101/85였다. 외부parser의단순byte→전체표해석은구간기준을누락한다. 구간기준복원후좌표연결185346코너의본이름집합은모두원본과일치했다.

- [최신 상세 보고서](reports/operations/odol-palette-mapping-20261109.md)
- [구조화 결과](data/odol-palette-mapping-20261109.json)
- [앞선255행렬셰이더선언](reports/operations/skin-shader-capacity-20261109.md)
- [기존 보고서](reports/operations/)

396코너좌표미연결,수치가중치/bindmatrix/실제엔진업로드미검증. parser해석차이를엔진손상/256한계로확정하지않는다. 기존전체모델시각실패와5파트기준유지. 새게임/모델변환/PBO없음. 성능·시각개선미측정,20ms목표미해결.
