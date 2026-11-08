# DayZ NPC — 현재 상태

최신 진단: 등반 시작 재시도 간격(0.15초).

|항목|판정|
|---|---|
|실제 Enforce 컴파일·시계 계약|통과|
|합성 64개 시계|2,624 입력, 912 재시도, 1,712 대기; 최장 0.20초|
|8/64 NPC 기능 회귀|미실행|
|CPU·Present·시각 개선|미측정|
|채택|보류|

현재 OpenTraversal은 이미 다른 개체 점유에 따른 등반 차단을 우회합니다. 혼잡 대기의 원인은 아직 특정되지 않았습니다. 합성 재시도 감소를 성능 개선으로 해석하지 마세요.

[상세 보고서](reports/operations/admission-cadence-contract-20261108.md) · [공개 수치](data/admission-cadence-contract-20261108.json)

이전 [네이티브 setter 비용 경계](reports/operations/preanim-setter-native-cost-boundary-20261108.md) 및 원시 연구 이력은 보존합니다. 다음은 8개체 실제 OFF/ON 검증이며, 통과 후 64개체 비용 비교입니다. 20ms p95는 목표입니다.
