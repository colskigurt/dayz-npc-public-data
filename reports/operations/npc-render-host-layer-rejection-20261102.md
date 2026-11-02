## 1. 작업 목표

실제8파트 network 후보의추가7개시각호스트가물리상호작용에중복참여하는지줄여볼수있는지최소격리시험으로확인한다. leader/피해receiver/원래관절·지지·월드·밀침·낙하판정은보존한다. 결과는후보수용여부를바꾸는시험이며사전성능개선을가정하지않았다.

## 2. 실제 수행

8NPC/64parts의같은765E PBO·기존5PBO·미션경로를사용했다. mission은opt-in flag가있을때서버의각NPC parts1..7에dBodySetInteractionLayer(host,PhxInteractionLayers.NOCOLLISION)를설정한다. dBodyIsSet는확인하고본체를삭제/비활성화하지않았다. part0leader·damage receiver·시계·그래프·solver는변경하지않았다. 클라이언트레이어를별도로변경하거나읽지는않았다.

SETUP/ACTIVE/TERMINAL/RESTORED 레이어와1Hz script시간·업데이트·머리MS포즈를기록하도록했다. SETUP의head는0placeholder로명시하고실제포즈표본으로사용하지않는다. 최초eight-layer0-1은Enforce의인덱스표현식컴파일오류로중단됐고보존했다. eight-layer0-2는명시headHandle지역변수로바꾼후컴파일됐다.

후속eight-layer-control1은동일mission바이트와관측기를유지하되레이어opt-in flag를만들지않았다. 초기ready대기도ON약93.9초보다긴OFF106.5초로두어단순초기대기만으로참조가사라지는지구분했다. 전경게임창은Computer Use로확인했다. 준비실패/후보실패/대조통과를각각기록한다.

## 3. 측정 결과

|항목|레이어변경ON|원래레이어OFF|
|---|---|---|
|초기물리본체존재|64/64|64/64|
|초기시각호스트레이어변경|56개|0개|
|초기원래레이어readback|524288|524288|
|변경후시각호스트readback|2|524288|
|실제경로기능시험|시작직후VM예외·guard종료|74.2852초에8/8유효종료|
|포즈/파트계약|파트참조소실·실패|64파트script시간·머리MS변화확인|
|최종원래레이어검증|완료불가|64/64|
|기록된FALL→LAND→FOLLOW|완결시험아님|9/9|
|전이/generation/terminal오류|완결시험아님|0/0/0;전이144|
|성능·표시품질|미측정/미수용|미측정/미수용|

ON은시작직후preCommit/partSpread가약11km까지벌어진표본과rendered.GetPosition의NULL instance예외가발생했다. 이는원점과의거리수준인비정상좌표관측이며실제11km이동/순간이동기능을구현한것이아니다. 바로다음레이어계약도거부돼guard가종료했다. native접근위반으로단정하지않고script VM예외와하네스강제종료로기록한다.

legacy폴더명layer0를숫자0의실제설정으로해석하지않는다. 사용한것은named NOCOLLISION이며현재런타임readback은2다. SDK enum의텍스트상순번을실제마스크값으로가정하지않는다.

## 4. 원인 분석

확인:동일관측mission의OFF에서는더긴초기대기후에도64개파트가존속하고8NPC경로를완료했다. ON은레이어변경후파트참조/위치계약이깨졌다. 현재방식의NOCOLLISION변경은수용할수없다. API가존재한다는사실과안전하게쓸수있다는판정은다르다.

미확인:내부어떤수명/공간등록/물리·네트워크처리가파트를소실시켰는지는모른다. 단일ON/OFF쌍이고위상/대기시각은완전일치하지않으므로모든엔진상태에대한원인확정은아니다. bodyIsSet초기true는이후수명·표시/본버퍼존속의보장이아니었다. 시뮬레이션을끄거나검사를강제성공시켜실패를숨기지않았다.

또한원래Floors/Walls mask는BUILDING/ROADWAY/TERRAIN/VEHICLE및일부장애물로구성된다. 추가호스트가기존Ray/Sphere비용의주원인이었다는증거는없다. 이번에그비용을측정하지못했으므로‘충돌56개제거만으로빨라진다’는결론은기각한다. 후보의기능실패와성능가설미측정을구분한다.

## 5. 검증 수준

현재레이어후보:TESTED_FAILED/수용기각. 원래레이어+동일관측기8개체:단일기능회귀통과. CPU모델공간head변화:진단성공;실제화면부드러움/graphclock의완전증명아님. 클라이언트레이어복제·피격/공격·64개체후보·반복성능·20ms목표는미검증이다. 실패후보를64까지확대하지않는다.

## 6. 변경 사항

격리mission/flag/런처/분석만추가했다. 새PBO는없다. 기존6PBO의해시·서명은각실행전검증성공했다. 원본게임소스/운영서버/Steam/자산/서명배포본/개인키는변경하지않았다. 서버·클라이언트시험프로세스는종료를확인했으며normalQuitVerified=false를정상종료로바꾸지않는다.

OFF분석의첫파생본은원래레이어readback을NOCOLLISION값처럼이름붙인필드가있었다. 원본파생본도남기고v2에서candidateEnabled=false/observedNamedNoCollisionValue=null과원래readback을분리했다. 공개집계는이정정내용을사용한다.

## 7. 원시 증거

sourceRunId 묶음:animation-network-role-20261101-1600/eight-part-20261102/network-gameplay/{eight-layer0-1,eight-layer0-2,eight-layer-control1}. plan·동일mission증명·기존PBO서명검증·서버/클라이언트launch·script/RPT/오류·render-host-layer/gameplay/state-contract/terminal TSV·OFF eight-analysis 및render-host-layer-analysis-v2를연결한다. 준비실패/부분기록/원래성공/이전64실패를보존한다. 공개packet은선정집계이며원시전체/프로세스정보/절대경로/키/영상/게임자산은게시하지않는다.

## 8. 잔여 문제

다음최적화는이레이어후보를계속확대하는것이아니다. 기존scratch/SupportFast의안전성근거와반복비용비교를우선하고,애니메이션경로에서는일반클라이언트호환성과실제화면판정을별도로확인해야한다. 내부파트소실원인분석은다른안전후보의설계를바꿀때만추가한다.

추가로기존sixtyfour1 pressure-plan-pose TSV에ID55의LAND직전/복구중4본MS포즈·heading·lift·idleOffset이있음을확인했다. 따라서직접상태전체복원은아니어도기존frozen-pose replay방식으로지형접촉일부를재현할자료가있다. peer/최종시점포즈/전체동적상태까지확보됐다고하지않는다. 새대용량수집없이기존자료를먼저활용한다. 전체골은미완료다.

로컬 checkpoint SHA-256: 27b5e1f2453b04316d0e5e214f594dff8f7f6d3f5324fa5b0faf1f270cf9d092
