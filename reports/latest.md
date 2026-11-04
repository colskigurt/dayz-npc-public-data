# 최근 판정 완료 실험 요약

sourceRunId: network-gameplay/terrain-scope-and-inline-cost · 2026-11-03

- 제한·인라인 각2독립 프로세스,8조건, 보고 묶음512000회. 같은 후보4반복 아님
- 평지 이득은 유지됐으나 두 후보 모두 배제 경로3% 기준 미통과. 게임 확대 없음
- [상세 판정](operations/npc-terrain-scope-cost-20261103.md) · [집계 JSON](../api/public/npc-terrain-scope-cost.json)
- 전체CPU/Present·8/64·화면 품질은 이번에 미측정.20ms p95 미달성

## 보존된 근거

- [이전 전역 후보](operations/npc-terrain-method-cost-20261103.md)
- [최근 기능 후보](operations/npc-entry-facing-regression-20261103.md)
- [평지 바닐라/커스텀 비교](operations/matched-flat-pursuit-20261102.md)
- [바이너리·메모리 도구 진단](operations/npc-native-analysis-barriers-20261103.md)

## 별도 정적 소스 분석

2026-11-04 [AntifreeZe·Expansion AI 분석](operations/npc-horde-ai-source-review-20261104.md). 새 실험0회이며 위 실험 날짜·수치·수용 판정은 바뀌지 않았다. [PZ 연결 복구](operations/npc-pz-bridge-recovery-20261104.md)도 별도 운영 기록이다.


## 현재 최적화 후속

[실제솔버경계·지지영역검사](operations/npc-terrain-boundary-review-20261104.md) · 현재DayZ1.30해시로분리.180대조/120rayhit,falseClear0.성능개선판정아님.다음은원본직접/동일래퍼/통합후보의동일버전비교.기존실패후보·게임간연구보존.
