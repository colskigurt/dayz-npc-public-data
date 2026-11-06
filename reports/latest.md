# DayZ NPC 현재 상태

최신: [접촉 시간계측 후보 Enforce 컴파일 확인](operations/contact-timing-compile-only-20261107.md).

| 단계 | 판정 |
|---|---|
|빌드·서명·packed소스|검증 완료;후보 기본OFF|
|Enforce/미션시작|서버 compile2 통과;클라이언트0·NPC0|
|분석기|합성 정상2통과·잘못된입력7거부|
|실제 솔버·8/64·CPU·시각|미실행·미수용|
|정상 서버 종료|실제-1로 정상종료 수용 아님|

내부TickCount 호출을 선택OFF할 뿐 물리 질의/판정을 생략하지 않는다. 컴파일 통과를 기능/성능 성공으로 해석하지 않는다. 전용 측정 시간 확보 전 비용시험은 보류한다.

[공개 JSON](../api/public/contact-timing-compile-only-20261107.json) · [후보 코드](../review/contact-timing-candidate/NFContactTimers.c) · [원래 설계](operations/contact-timing-candidate-prepared-20261107.md) · [SupportFast 미수용 기록](operations/supportfast64-cost-interim-20261107.md)
