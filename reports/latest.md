# 현재 상태 — 64개체 CPU 최적화 계속, 수신체 축소 후보 보류

낮은 등반의 Head 충돌 광선 대조에서 원본/원본 0/1,947건, 후보/원본 12/1,914건, 원본/후보 12/1,881건 불일치. CPU 포즈 동등성이 충돌 동등성을 보장하지 않았다. 내부 원인 및 실제 총기·근접 피해 영향은 미확인이다.

27노드 후보는 채택하지 않는다. 기존 클라이언트 비용 감소와 서버 CPU 목표 달성은 별개이며, 전체 CPU 기준은 미통과다. 이번 시험의 비용·화면 품질은 미측정. 기존 밀침·낙하·재등반 기능과 배포본을 보존한다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/receiver-fire-ray-climb-hold-20261109.md)

<details><summary>직전 요약 보존</summary>

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

[실제 엔진 보고서](operations/foot-ray-range-native-rejected-20261109.md) · [공개 수치](../data/foot-ray-range-native-rejected-20261109.json) · [앞선 조건부 수학검사](operations/foot-ray-range-feasibility-20261109.md)

기존 [그래프캐시64CPU](operations/graph-write-clean-cost64-screen2-20261109.md)·셀풀은미수용, [후열감속](operations/rear-pressure-shadow-20261109.md)은보류입니다. 지속등반·밀침·낙하·재등반보존. 전체64기능·시각·CPU최적화는미완료이며20ms p95는목표입니다.

</details>
