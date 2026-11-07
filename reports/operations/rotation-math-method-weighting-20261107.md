# 회전 산술 메서드 — 동일 좌표·국소 효과 확인, 전체 병목 후보는 후순위

## 1. 작업 목표

기존direct-computation-feasibility의같은heading 회전중복후보를큰구현전에선별한다.소스수준16개sin/cos표현을2개로줄이는산술이좌표를보존하는지와전체비용상연구가치를확인한다.물리질의삭제나새보간·업데이트방식은적용하지않는다.

## 2. 실제 수행

원본PBO의DGBNativeGroundBody.Rotate를직접호출하는경로와같은각도의sin/cos를미리계산하는4점경로를기존서버메서드하네스에서비교했다.128벡터패턴×64각도×2원점×4점,총65536좌표를세성분숫자동등으로비교했다.실제NPC/클라이언트0,새PBO0이다.

method1은뺄셈표기파싱오류로실행전실패·소유서버종료기록보존.method2는좌표동등을통과했지만시간블록간checksum누적으로비용비교를잠정자료로만남겼다.method3은checksum을각블록0으로초기화하고동일출력확인후OFF/ON3쌍을실행했다.이전실행재분석을독립반복으로세지않는다.

## 3. 측정 결과

65536좌표숫자불일치0,최대거리오차0.각블록50000개의4점회전,한프로세스3쌍의경과Tick 변화는약−41.40%,−46.02%,−41.79%,중앙약−41.79%.수용전메서드선별기준을통과했다.이는native실제호출수나CPU self-time측정이아니다.

외부QPC·GetProcessTimes로전체벤치마크를포함하는구간을측정했다.0.45328벽시계초동안총프로세스CPU406.25ms.이안에는원본150000그룹+후보150000그룹·대기·파일·기타엔진작업이포함된다.

기존64평지의body solve2564.88/게임초에전체406.25ms를원본150000그룹에모두배분해보는보수적규모환산은약6.95ms/게임초다.이는실제전체절감률이나프로덕션상한이아니고,합성회전그룹이실제4점회전을대표한다는조건부규모점검이다.전체서버CPU기준약788ms/게임초와의차이를볼때이산술만으로주력병목을해결한다고볼근거는없다.현재전체CPU개선측정0,p50/p95/p99신규0.

## 4. 원인 분석

확인:명시적같은heading 산술재사용은합성범위에서동일좌표를내고메서드경과Tick을줄였다.단순상대표현감소가컴파일러에의해완전히자동제거된것과같은성능은아니었다.그러나구체적인native콜횟수·컴파일러내부최적화는미관찰이다.

미확인:14클립실제본포즈전체·signed zero비트/NaN·통합솔버/64최악조건효과.로컬−42%를전체CPU−42%로바꾸지않는다.메서드gate는통과했지만전체기여규모가작을가능성이높아새PBO·8→64시험을자동생성하지않고후순위보존한다.이는산술후보전체를반증한것이나기능실패가아니다.

## 5. 검증 수준

숫자동등진단·한프로세스3쌍메서드선별통과.독립반복게임성능·기능회귀·시각·최종수용은미실행.서버RequestExit0후실제exit−1,정상종료gate는통과아님.현재시험서버·클라이언트·수집기없음확인.전체골active유지.

## 6. 변경 사항

solve-rotation1 내시험미션·CPU containment helper·manifest·결과만생성.원본PBO `453d6a9e688d7bf14c5b98c305dd5a45200ec52156949c10691839b43c58bc9a`를읽기전용사용.실제NPC소스·PBO·애니메이션·물리판정·운영서버변경없음.새서명대상없음.실패/잠정기록은덮어쓰지않았다.

## 7. 원시 증거

ANIM-rotation-math-method1/2/3,method3의rotation-math.tsv·rotation-clock.tsv·benchmark-process-cpu.json·mission·manifest·script로그·terminal server record,results.json,사전acceptance.json.원시파일무결성확인후공개집계와상대증거ID/해시만게시한다.

## 8. 잔여 문제

접근조기탈락은64전체비용screen실패,회전산술은로컬효과가있지만규모상후순위로정리한다.더작은함수의좋은퍼센트를찾는것으로주력목표를대체하지않는다.다음구조적검토는기존5표시파트/피해수신기호스트의실제물리참여범위를현재소스/기존연구와대조하는것이다.생성시PhysicsBody와내부15쌍block만으로실제충돌CPU비용을확정할수없다.공식API에는per-body layer/solid/active 조회선언이있지만현재Animal호스트의값·네이티브제어기효과·스크립트설정의복제는미확인이다.변경하기전읽기전용값확인과기존실패중복조사를먼저해야하며시뮬레이션/충돌을임의로끄지않는다.

공식선언참고: [enphysics.c](https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/main/scripts/1_core/proto/enphysics.c), [PhxInteractionLayers](https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/main/scripts/3_game/global/dayzphysics.c).공개main선언은현재설치바이너리동작검증을대체하지않는다.
