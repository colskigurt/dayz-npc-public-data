# 그래프변수중복쓰기 후보 — 기본OFF빌드·계약미검증

## 1. 작업 목표
4파트비용후보미수용후새로운국소후보를선정한다.접촉안전검사를삭제하거나기능복구개발로전환하지않는다.
## 2. 실제 수행
기존cost-coverage/support-entry/무변위Displace가드와기록을검토했다.현재DGNativeCommand.RunPreAnim은값불변에도RequestedState/TraversalReleased/LandingConfirmed setter를호출한다.선정기록에서는변경시쓰기캐시실험을확인하지못했다.미실행전체증명은아니다.
기본OFF격리모듈을만들어명령별캐시·최초쓰기·값변경쓰기·OnActivate무효화를구현했다.추가정책/쓰기블록을역변환하면나머지RunPreAnim이기준본문과정확히같음을검증했다.기존서명키를복사하지않고빌드·서명·검증했다.게임미실행.
## 3. 측정 결과
PBO15b2ea7fb8801900c40d3985e3fb487db73a6295fbc1dd6e19b7c41114fb9962.서명검증성공·개인키미포함.설정문법통과이나Enforce컴파일/런타임미검증.
이번8/64기능·CPU·Present p50/p95/p99·시각수치없음.예상절감률미제시.
## 4. 원인 분석
확인:무변위Displace는기존가드가있어새후보가아니다.공식API는setter를OnActivate/PreAnimUpdate에서사용하도록선언한다.값의콜백간보존·외부쓰기정책은이선언만으로확인되지않는다.
가설:같은값의반복스크립트-네이티브쓰기를줄이면비용이줄수있다.실제setter순수비용·엔진내부중복제거여부미확정.캐시분기비용이이득을상쇄할수있다.
검증설계수정:기존14클립CPU씬은Review_0..13직접시작노드라실제게임플레이변수보존증명이될수없다.그대로재실행하지않았다.
## 5. 검증 수준
소스국소변경정합성·빌드서명검증성공.기능·시각·비용·최종수용미실행.4파트의미수용판정유지.
## 6. 변경 사항
새격리기본OFF관찰후보만추가.운영PBO·원본자산·물리·갱신주기·시간누적기·본평가미변경.초기줄바꿈치환실패는소스생성전assert로중단돼보존했고CRLF정규화후빌드했다.기존공개보고서/원시증거보존.
## 7. 원시 증거
graph-variable-write1/preflight.json,source-contract-review.json,stage-failure1.json,build.json,signature.log,reference-RunPreAnim.c.txt,후보소스.기준DGNativeProbe.c SHA256 b1f51ffbd179782676829bfb3045a62c7a1202caf40847504af60057f967ed86.
## 8. 잔여 문제
현재candidate-only선택기는기존Review타입전용이라실제그래프쌍시험의명시적개체선택으로바꿔야한다.다음단일산출물은실제게임플레이그래프에서hold/request/release/landing전환및재활성화의보존을확인하는작은CPU포즈계약시험이다.불일치면기각.일치해도8→64기능·전체CPU·시각판정은별도다.현재setter캐시는실제병목해결책으로입증되지않았다.
