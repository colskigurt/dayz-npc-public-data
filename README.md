# DayZ NPC — 현재 상태

2026-11-09: 발 지지 광선 범위 후보를 실제 엔진에서 검사했고 미수용으로 종료했습니다.

|항목|결과|
|---|---|
|실제 Bullet 비교|2,610조건|
|lift 허용차 초과|45조건·최대0.305mm|
|0.25m 수용/거부 변화|이번표본0건 — 전체동등성증거아님|
|비용 반복/8→64 확대|동등성게이트실패로미실행|
|원본코드/PBO/게임플레이|변경없음|

수학모델은 통과했으나 보존구간 반환값이같다는가정이실제출력에서성립하지않았습니다. 사후허용오차완화나미측정성능주장은하지않습니다.

[실제 엔진 보고서](reports/operations/foot-ray-range-native-rejected-20261109.md) · [공개 수치](data/foot-ray-range-native-rejected-20261109.json) · [앞선 조건부 수학검사](reports/operations/foot-ray-range-feasibility-20261109.md)

기존 [그래프캐시64CPU](reports/operations/graph-write-clean-cost64-screen2-20261109.md)·셀풀은미수용, [후열감속](reports/operations/rear-pressure-shadow-20261109.md)은보류입니다. 지속등반·밀침·낙하·재등반보존. 전체64기능·시각·CPU최적화는미완료이며20ms p95는목표입니다.
