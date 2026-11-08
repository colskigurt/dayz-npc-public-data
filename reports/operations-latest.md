# 공식프로파일러 시간단위·평균 검증

## 1. 작업 목표
현재CPU후보를호출횟수만으로고르지않도록공식프로파일러의시간단위·평균·부모자식중첩을확인한다. 기존후보중복조사를먼저수행하고이미미수용인셀풀은재시험하지않는다.

## 2. 실제 수행
기존셀풀engine-26220720/cost64-three2-resume2/REPORT를읽어유효2쌍Present p95가각+14.95%/+11.36%로채택실패했음을확인했다. 부분파일럿의이득을최신결론으로재사용하지않았다. 방향setter/지지Fast/수신기골격등기존기록도대조했다.
공식enprofiler.c를읽으니SetTimeResolution/GetTimeResolution 및EnableAverage/IsAverage가명시되어있었다. 이옵션확인이기존분석에빠져있었음을정정한다. 공개소스는LF해시가다르지만CRLF변환해시가이전설치SDK의ba0bdef9…와정확히일치했다. 따라서관련API내용은새버전에서추가된기능이라고설명하지않는다.
이미작동한NPC없는DayZDiag -server 경로를재사용했다. Parent50회가Child100회(64회sin계산)를호출했다. 같은누적데이터를해상도100000/1000/1과평균ON/OFF로조회했다. 모드/PBO/키/보안설정변경과게임클라이언트는없었다.

## 3. 측정 결과
새프로필초기resolution100000,average=false를실제읽었다. 동일데이터의Parent/Child 시간은100000해상도48.58/46.77,1000해상도0.4858/0.4677,1해상도0.0004858/0.0004677이었다. 1000해상도값은ms이며100000해상도한단위는10마이크로초다.
평균ON의Parent/Child 0.00991429/0.0095449는누적ms를sessionFrames49로나눈값과일치한다. profiledFrames48로나누면안된다. 이번직접함수조회count는평균ON에도50/100으로그대로여서시간평균의효과를호출count에확대하지않는다.
부모0.4858ms와포함된자식0.4677ms의중첩을관찰했다. self-time이나서로독립된시간으로합산하지않는다. 새NPC/8/64기능시험0·성능/Present/시각개선측정0.

## 4. 원인 분석
확인:공식지원조회옵션으로시간단위와평균을명시할수있고현재진단이미지에서반영된다. 기존rawTime이해석불가능한바이너리값이라는전제는수정한다.
한계:과거NPC실행은당시resolution/average옵션을저장하지않았다. 같은이미지/동일SDK와새프로필기본값이있더라도과거값의설정을소급확정할수없다. 과거raw475279를무조건4752.79ms라고공식비용으로수정하지않고기본값이었다는조건하에서만추정한다.
기존동일실행의상대함수순위는공통스케일에서참고가능하다. SolveContactsInternal/Drive/Follow/Displace/Update는중첩되므로부분비용을합산하거나프로세스전체CPU와동일시하지않는다. diag에서시간이큰것을retail절대비용으로복사하지않는다.

## 5. 검증 수준
프로파일러단위/평균/중첩진단성공1회. 최적화효과·NPC기능/시각품질·반복성능/최종수용아님. 서버자진RequestExit후프로세스소멸/exit−1·강제종료없음을기록하며exit0정상종료로바꾸지않는다.

## 6. 변경 사항
독립init.c와조회결과/해시/분석/보고서만추가. 기존NPC게임코드/PBO/모델/실행기/운영서버불변. 새서명대상없음. 기존승인거부통합어댑터명령을재시도한것이아니며이미성공한NPC없는프로브경로만사용했다.

## 7. 원시 증거
ANIM-enprofiler-units1. plan/launch/terminal,units.tsv,analysis.json,공식소스원본과api-source-reference.json보존. 이전SDK해시와CRLF동일성은analysis.json에추가연결했다. 진단exe4bbf370b…는이전API프로브와같은이미지다. 함수호출수50/100·해상도비례·평균분모49를파생분석의assertion으로검증했다. 기존셀풀2쌍실패및NPC64함수순위자료는새독립측정으로세지않는다.

## 8. 잔여 문제
다음비용분석은시간단위1000/평균false를명시하고해당옵션·session/profiledFrames를같이기록해야한다. 과거자료는단위미확정표시를지우지않고이진단을연결한다. 새큰NPC실행을즉시시작하지않는다. 같은상위콜체인에속하지않는제거가능한비용을기존순위/소스로선정하고실제수정가능성을먼저판정한다. 셀풀·단순Drive/Displace합성·지지검사삭제등기각방법을반복하지않는다.
공식근거: https://raw.githubusercontent.com/BohemiaInteractive/DayZ-Script-Diff/main/scripts/1_core/proto/enprofiler.c
