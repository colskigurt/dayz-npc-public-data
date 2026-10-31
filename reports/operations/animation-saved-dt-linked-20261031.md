# 실제 입력 dt와 native 시간 회계 연결: 단일 진단

작성: 2026-10-31T17:06:25.833036+09:00

## 1. 작업 목표

수정한선택조건으로실제저장입력dt→누계→평가→잔여시간→보간분자를연결한다.이전수치보정은그대로유지하고,CPU포즈도독립source FK와대조한다.새제품보간이나배속을추가하지않는다.

## 2. 실제 수행

기존서명B8FB·453D·0C6E로고정1개체100초실행 runtime-20261031-170014를진행했다.미션6.167초·본갱신102회·실제queried/전경포즈를확인한뒤30초native관찰과20초독립nonce/ACK시계연결을시작했다.선택된8파트의기존carry진단은동일하며inner에서saved XMM6 stack scalar를읽었다.직접caller ABI·동일frame소유renderer/descriptor/graph/numerator를대조했고,다른hook이있는상위반환주소를원본주소로가정하지않았다.

REA Frida는앞선바인딩누락상태를복구하거나재설치하지않고기존Python Frida를사용했다.REA Evidence가아니다.관찰후hook비활성화·게임정상종료·원시입력무결성을확인했다.영상녹화없음.CPU6관절source FK는기존quarter-key독립fitter의경로만맞춰재사용했고candidate graph clock으로phase를선택하지않았다.

분석파일목록에configuration.json sidecar가빠져최초분석저장이실패했다.실행된instrumentation의정확한inline JSON을추출해보완했으며실험을다시실행하거나원시로그를수정하지않았다.파생경위를별도로보존한다.

## 3. 측정 결과

| 항목 | 결과 |
|---|---|
| active linked outer/inner |1499건|
| 연속float32회계 |1498/1498 정확일치|
| 연속구간evaluation |599회|
| 독립clock ACK |39건|
| CPU callback과QPC결합 |1499건|
| 입력dt p50/p95/p99 |20.000/20.000/21.000ms|
| 입력dt범위 |17.000~23.000ms|
| 입력dt와effective step차이RMS |0.000400730ms|
| inner→다음CPU callback p50/p95/p99 |18.5874/19.3730/19.5125ms|
| outer→inner p50/p95/p99 |0.0189/0.0217/0.0363ms|
| script/observer오류·drop·long-frame guard |0/0/0/0|

실제입력dt는이전CPU interval과차이RMS0.2462ms,다음CPU interval과0.4502ms였다.현재native-entry wall interval과는1.8229ms이고상관0.0747이다.입력대부분이20ms인좁은분산조건이므로상관계수만으로동기화성패를판정하지않는다.콜백타이밍과스케줄러에전달된dt는다른측정이다.

독립CPU포즈추정:앞뒤250ms제외1473표본/1472연속쌍,phase/mission=0.999750982,역방향단계0.6관절FK오차p50/p95/p99=8.299/20.318/26.463mm.추정phase step p50/p95/p99=20.833/25.000/33.333ms,최대58.333ms다.이것은4.167ms해상도최근접source FK추정치이며실제표시frame지연이나확정된정지시간이아니다.100초중관찰30초만분석하며1개체를8개체로세지않는다.

## 4. 원인 분석

확인:직접읽은저장입력dt를float32로더한누계가평가전total과같고,threshold교차·50ms graph step·잔여시간·보간분자가1498단계모두맞았다.따라서기존후보에서미확인이었던입력회계연결은이1회표본범위에서확인했다.독립CPU포즈추정평균진행도약1배이며숫자graph clock만맞춘결과와구분한다.

미확인:source phase추정의tail에는FK모델·보간·phase양자화오차가섞인다.이를화면끊김으로단정하지않는다.이번영상없는실행의p95 callback대기19.37ms와이전녹화실행약33ms를차감해OBS가원인이라고주장하지않는다.녹화/계측/실행순서조건이다르며matched comparison이아니다.지원제품경로·GPU palette/Present소비·실제smoothness는여전히미확보다.

반증범위:후보의시간회계가실제incoming dt와맞지않는다는해석은이측정에맞지않는다.이것이모든고부하/긴frame/전투/14클립에서맞는다는의미는아니다.입력17~23ms조건으로긴framebacklog안전성을수용하지않는다.

## 5. 검증 수준

갱신준비gate·observer소유권·saved-dt f32회계·독립CPU평균진행:단일진단성공.기존4회carry인과진단과연결하지만이번새직접dt관찰의반복수는1이다.시각품질·GPU소비·지원배포수정·8/64회귀·성능목표·최종수용:미완료.20ms p95는목표다.

## 6. 변경 사항

새observer/driver준비gate와분석·보고파생물만추가했다.기존게임Source·미션·PBO·운영서버·원시과거자료·개인키는보존했다.새PBO/서명생성없음.실행전기존3모드서명검사통과.후보carry는격리native진단이며제품배포가아니다.8파트4800회numeric carry기록,descriptor pointer교체없음,active return hook없음이다.

## 7. 원시 증거

plan/readiness/inline configuration/observer/driver/calls/status/clockbridge/result와profile launch/CPUclock을연결한다.source FK TXA는기존manifest해시검증후사용했다.패킷은집계와선정처음/마지막모델·CPU쌍이며전체raw/영상이아니다.소스파일text정규화해시와디스크byte해시는구분하며publisher는실제byte해시를검증한다.실험정상종료뒤저장한다.

## 8. 잔여 문제

다음은source-phase추정기자체오차와실제buffer/화면소비를구분하는일이다.추정phase tail을근거로새보간을추가하지않는다.녹화없는실행과기존녹화자료의차이는후속통제연구후보일뿐이다.지원되는실행경로와14동작·8/64안전성·부하비용은별도gate다.정상낙하·착지·FOLLOW·재등반및이전LOW자극반복미수용판정은변경하지않는다.

선정공개본문.로컬보고서SHA-256:`0a3d2f3a150540c14344d5856c3dbd1bdec29cb0c50291fdf0c0df013d05dd3c`.전체raw가아니다.
