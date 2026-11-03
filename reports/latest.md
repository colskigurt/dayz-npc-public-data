# 최신 실험 요약

기준:2026-11-03 · sourceRunId: network-gameplay/terrain-method-cost

- 기존8모듈·서버 전용·2독립 프로세스·128000회 메서드 비교
- 평지 솔버 경과시간 약55~60% 감소, 지붕/가장자리 약21~38% 증가
- 전역 기준 미통과·후보 미채택. 전체CPU/Present·8/64·화면 품질은 이번에 미검증
- [비용 보고서](operations/npc-terrain-method-cost-20261103.md) · [집계 JSON](../api/public/npc-terrain-method-cost.json)

## 기존 기능·전체 비용 근거

- [최근 기능 후보8/64](operations/npc-entry-facing-regression-20261103.md):8/8,64개체 전체 이력53/64,낙하82/82복귀. 단일 실행
- [과거 평지4회 비교](operations/matched-flat-pursuit-20261102.md):fresh/continued·Present/서버dt 구분
- [현재 상태](../README.md) · [채택 기준](operations/npc-validation-policy-20261103.md)

정상 밀침·낙하·복귀는 허용한다.20ms p95와 시각 품질은 미수용이다.
