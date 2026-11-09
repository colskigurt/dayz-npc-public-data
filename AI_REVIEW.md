# DayZ NPC Observatory — 현재 상태

2026-11-09: **골격 크기만 분리하는 입력의 정적 관문 통과.** 정상48가중본모델의전체LOD바이트를유지하고P3D평면골격목록만48→248본으로확장했다. 실제ODOL에서도248본,가중48본/하위표48,모든LOD속성·수치가중치·bounding center동일을확인했다. 새격리PBO는서명/검증성공.

- [최신 상세 보고서](reports/operations/cardinality-input-gate-20261109.md)
- [구조화 결과](data/cardinality-input-gate-20261109.json)
- [구간별본매핑 대조](reports/operations/odol-palette-mapping-20261109.md)
- [255행렬 셰이더선언](reports/operations/skin-shader-capacity-20261109.md)
- [기존 보고서](reports/operations/)

아직runtime/공통CPUpose/화면/성능미실행이다. 전체몸체1host지원·성능개선확정아님. 원본PBO·모델·애니메이션·물리검사유지. 다음은같은fullinstance를쓰는고정1쌍runtime진단이며8→64로자동확대하지않는다.20ms p95목표미해결.

249본전체계층은부모관계도바뀌므로선택하지않았다. 실제기존flat목록248본을사용했고새메모리점0,packed클래스는기준상속/model경로만변경한다. config/build서명성공을Enforce/runtime/실제표시성공으로취급하지않는다.
