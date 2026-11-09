# 현재 상태 — 64개체 밀집 구간의 접촉 처리와 CPU 동시 관측

| 목표 위치 | 서버 전체CPU ms/벽시계초 | Displace 호출/초 범위 | 기본 Move 호출/초 범위 | 표본 근접쌍 평균 |
|---|---:|---:|---:|---:|
| 지상 |677.95|762.46~815.31|924.48~998.17|54.68|
| 낮은 발판 |831.14|1114.89~1176.53|922.35~981.66|324.68|
| 높은 발판 |862.25|1085.73~1179.16|774.65~838.69|279.46|

**CPU ms/초는 프레임ms가 아니다.20ms p95와 직접 비교하지 않는다.** 동일70초미션 중50초를1회진단했다. 카운터범위는완전64표본의직전/직후경계로계산했고전환5개구간은분리했다. 목표위치와NPC현재위치는구분한다. 근접쌍은실제native manifold 수가아니다.

- 관찰:발판목표에서기본Move빈도증가없이Displace와CPU가함께증가했다. 접촉처리를주력비용후보로유지한다.
- 미확정:순수쿼리/Displace self-time·네이티브접촉/로깅/다른스레드의기여. 인과A/B가아니다.
- 기능:최종 VALID_TERMINAL3/64·INCOMPLETE61/64,기능회귀미통과. 기능복구개발로범위를전환하지않았다.
- 시각·성능:Present/실제화면미측정·새최적화미적용·최종수용미완료.
- 기존판정:서버LOD클라이언트포즈최대0.261m로기각. 물리마스크runtime의도일치. 과거실패후보보존.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/interval-contact-cpu-context-20261109.md) · [마스크·중복 후보 검토](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/physics-mask-and-candidate-review-20261109.md) · [클라이언트 포즈 gate](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/client-role-pose-gate-failed-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)

다음은기존네이티브접촉관측기의CONTACT이벤트등록·양성대조계약을감사해,선택적표시호스트간충돌차단이검증가능한지결정한다. 이벤트0을비용0으로보지않고같은0관측을반복하지않는다. pair-block이나전역layer변경은아직구현하지않는다.
