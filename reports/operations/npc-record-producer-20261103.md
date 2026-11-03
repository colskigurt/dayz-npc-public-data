## 1. 작업 목표

문제의16바이트 입력을 실제로 만드는 서버 호출과 클라이언트 수신 바이트를 연결한다. 생성·해석 계약과 전송 중 변화를 구분한 뒤 수정 가능 범위를 판단한다.

## 2. 실제 수행

기존1논리개체8파트 격리 하네스를 재사용했다. 서버 c6a1e0을 thread 단위로 추적해 내부 c10120의 mode4/5,full writer c0b130 호출수,a7e200 큐의 길이·전체 바이트·caller를 같은 호출 안에서 수집했다. 클라이언트는 paused spawn→observer load→resume로 초기 입력 전에 연결하고 선언된 레코드 전체를 최대128바이트까지 읽었다. 게임 필드에는 쓰지 않았으며 후킹 실행을 성능 비교에 쓰지 않는다.

기존3함수 REA 분석은 약17분 뒤3/3 결과·정상 close/server exit0을 반환했고 원본A500 해시 불변을 확인했다. 같은 초기화를 중복 재시작하지 않았다. 큐 함수는 length+4 이상을 할당해 length를 저장하고 해당 길이만 memcpy한 뒤 포인터 배열에 넣는다. 이 정적 흐름을 실제 malloc 횟수/CPU self-time/GC 측정으로 취급하지 않는다.

## 3. 측정 결과

서버8 receiver에서16바이트 레코드8건과 뒤이은3바이트 zero 레코드8건을 관찰했다. 모두 caller c6a487,실제 부모 없음,c10120 mode4=1/mode5=1이며 같은 emitter 안에서 full writer c0b130 호출은0이었다.16바이트는8건 모두 같은 내용이고,클라이언트의 비정상 부모 입력8건에서 받은 유효16바이트 전체와8/8 일치했다.

payload SHA256=053e741e48ca5e1f03c7d9ceb9d07036d5434ac7fc7073abb3257b351b70dea4. raw는로컬에보존한다. 내용이같으므로8개의network entity ID/time을각각일대일매칭한증거는아니다. 이표본에서생성후바이트변화는관찰하지못했지만transport전체의무결성을일반화하지않는다.

기존 declared16byte/128bit 대비146bit 종료·18bit 초과와ID가먼저범위내에서잘못읽힌관측을유지한다. 이번은producer연결이며codec원인확정이아니다. 초기111byte full-state writer와16byte emitter를같은경로라고설명하지않는다. 새8/64기능·시각·Present/CPU/GPU p50/p95/p99·할당/GC 수치는없다.20ms p95는목표다.

## 4. 원인 분석

확인: server native creature update→shared dispatch→queue에서16byte가생성되고,그전체바이트가client의비정상ID/후속범위초과입력과일치한다. 수신후임의값변조만으로설명하지않는다. full-state writer c0b130은문제의짧은레코드생성중호출되지않았다.

정적확인: c0b130에는guard c0bc40과header c0e7f0 뒤고정pose필드를쓰는경로가있다. creature emitter c6a1e0은mode0x101 context를만들고c10120뒤실제cursor로길이를계산해큐에전달한다. 큐단독이임의로16byte로축소했다는증거는없다. 생성계약이옳다고확정한것도아니다.

미확인: 같은dispatcher가쓰기mode1/읽기mode0에서선택하는분기·delegate,짧은레코드와기대reader형식이맞지않는이유,공식mod API의생성/명령계약으로고칠수있는지아직모른다. engine내부수정이필수라고결론내리지않는다. 새질문에해당하는c10120/c0e7f0/c0bc40 분석을시작했다. provider는close시ephemeral DB를삭제하므로재초기화가필요하지만완료된3함수는다시요청하지않았다.

## 5. 검증 수준

16byte producer실행·같은run전체바이트일치:제한진단성공. 완료3함수decompile:진단성공. ID/time포함end-to-end대응·원인확정·수정·8/64회귀·시각·성능반복·최종수용은미완료다. 후킹실행과과거unhooked성능을직접비교하지않는다.

## 6. 변경 사항

격리미션·런처·observer·분석JSON·보고서만추가했다. 포트치환은64자리해시문자열을제외하고실행전상수불변을검사했다. 원본binary·게임코드·9PBO·자산·Steam·운영서버는변경하지않았다. 새PBO/신규서명대상없음. 기존9PBO해시·서명검증은실행전통과했다. 진단후functional=false퇴역으로종료했으며소유프로세스정리를정상게임종료로보고하지않는다.

## 7. 원시 증거

run ID network-gameplay/creature-parent-producer1. server ev_ca354571dad93ebd2311ca26b904635007c0b33d917c8af36bac23d734745cf1,client ev_8c78944da3807e999634a0899d1576d16f2aa2ae1847032625f5d396f4df3d7e. 완료static batch ev_269a72b0447764460393bacacc91b4bfec1ae7be3c979e074875eb7a6b306ccf. raw·비교·source/byte guard·spawn/load/resume/status/close·서명launch·전체decompile/close를해시에연결한다. 평문observer-armed.json은별도유효JSON에내용/해시를기록했다. 실행중codec batch증가파일은완료증거에포함하지않는다. 공개는집계·상대주소·판정만제공한다.

## 8. 잔여 문제

다음은공유codec의최초불일치를특정하고mod API로수정할국소후보·이점·기능위험·검증비용을제시하는것이다. ID0강제·레코드무시·world/support검사삭제·물리비활성화로성공을만들지않는다. ETW준비후비용OFF/ON은hook없는8→64로확인한다. 정상낙하·착지·FOLLOW·재등반을보존하고전체goal active를유지한다.

로컬checkpointSHA256: 0e0d7202de827e0d32eaa8e39aac855b0fa22a8e0b1ab0add78d4208b7727490
