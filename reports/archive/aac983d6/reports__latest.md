# DayZ NPC 현재 상태

최신: [64개체 비용시작 전 크래시와 분리진단](operations/contact-timing64-client-crash-smoke-20261107.md).

| 항목 | 판정 |
|---|---|
|타이머OFF8기능|후보8/8·낙하9/복귀9;대조HIGH횟수조건미완료|
|64비용|첫대조군워밍업중크래시;CPU/Present수집0·비교0쌍|
|오류|client RVA0x21a110,20바이트복사중쓰기;소유객체불명|
|과거기록|후보없던실험에동일RVA범위오류;동일근본원인미확정|
|관찰기OFF진단|64수신기생존60.0057초,새덤프없음;단일생존일뿐|
|최적화/시각/전체20ms|미수용·미완료|

기존PBO·NPC물리·네이티브메모리불변. 관찰기OFF를영구수정으로채택하지않는다. 재시작으로유리한비용값을찾지않고실제크래시문맥을먼저좁힌다.

[공개 JSON](../api/public/contact-timing64-client-crash-smoke-20261107.json) · [8개체 결과](operations/contact-timing-eight-pair1-20261107.md)
