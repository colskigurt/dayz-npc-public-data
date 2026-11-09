# 현재 상태 — 선택적 표시 파트 충돌 차단의 8개체 회귀 통과

| 항목 | OFF | ON |
|---|---:|---:|
| 최종도착·전체경로 |8/8|8/8|
| 낙하·착지후FOLLOW복귀 |9/9|9/9|
| 상태/복구세대/종결정렬오류 |0/0/0|0/0/0|
| 추가차단생성/해제요청 |0/0|672/672|
| 완료게임시간 |64.5732초|73.8830초|
| 전체bodySolve누적 |18846|21284|

**기능 단일쌍 통과이며 최적화 채택이 아니다.** ON완료시간은14.4174%길었고CPU/Present/실제화면부드러움은미측정이다. 전투OFF·표시40파트·피해수신체0의이동시험이며피격/사망/재생성수명은미검증이다.

- 최초해제경로는접근위반으로실패했다. 원시덤프/소스보존후typed필드+지역인자경로로수정했고672개설정/rollback/해제및실제8이동뒤해제를확인했다. 정확한엔진내부원인은미확정이다.
- 해제API는void이므로local0/해제반환을native테이블잔여0·누수0으로확대하지않는다.
- 기존코어/PBO/모델/그래프는불변이고격리미션에는실제선택차단코드를추가했다.매프레임추가pair순회없음.
- 다음은동일조건64CPU선별계획이다.48384핸들생성/정리·메모리·진행률도검토한다.CPU유망시실제Present/시각및반복비교가필수다.
- 20ms p95는목표이며전체기능·시각·성능최종수용은미완료다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/selective-peer-block-eight-20261109.md) · [최소 수정 helper 코드](https://github.com/colskigurt/dayz-npc-public-data/blob/main/evidence/changes/selective-peer-block-helper3.c) · [직전64 CPU/접촉 구간표](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/interval-contact-cpu-context-20261109.md) · [보고서 전체](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
