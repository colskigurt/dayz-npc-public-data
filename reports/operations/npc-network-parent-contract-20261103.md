## 1. 작업 목표

커스텀 네트워크 반복경고가 바닐라에서도같은규모로발생하는지기존자료로확인하고, 미해결참조가실제부모연결인지공식바인딩과대조한다. 부모없는서버개체의클라이언트등록상태를읽어다음수정질문을좁힌다.

## 2. 실제 수행

기존닫힌RPT6개와초기mode2관련3개를스트리밍·SHA/파일안정성검사했다. 오프라인/네트워크·일반/진단client·단일/8파트를구분했으며새성능반복으로세지않았다.

원본A500의AddChild/RemoveChild/GetParent문자열등록과nativehandler를bounded Capstone으로읽었다. IEntity AddChild handler2f4a10은vslot80,RemoveChild2f5c50은slot78을소비한다. 이들은대기loop의호출slot과같다. IEntity GetParent의등록2fb133→leaf2f9960은object+68의component에서+18을읽으며실제loop의부모비교와일치한다. 다른GetParent동명handler2f9970(+58)을IEntity것으로혼동하지않았다. leaf가pdata에없어기존intervalassert가실패한뒤등록entry에서첫ret까지제한decode했다. SDK의공식선언과대조했다.

기존1NPC/8파트미션에생성직후공식GetParent/GetNetworkID/GetHierarchyPivot/IsHierarchyPositionOnly의서버TSV쓰기만추가했다. 클라이언트등록hook에같은GetParent필드읽기를추가했다. 최대8건뒤detach·55초window/60초CLIcleanup,기존10PBO·SETUP후보OFF. 부모/ID/native시계쓰기나계층강제연결은없다. 시험종료후보고서를저장했다. 입력처리함수후속MCP요청은아직대기중이며재시작/취소하지않았다.

## 3. 측정 결과

|기존실행|반복경고를포함한관련경고총건수|해석범위|
|---|---:|---|
|network vanilla 첫실행/독립후속|각1|둘다공통sentinel경고1건,문제반복ID없음|
|old custom network pilot4|1|커스텀실행역할실패가있어정상기능대조로쓰지않음|
|matched offline main/main2|각0|네트워크계약대조가아님|
|초기단일파트·일반release client|641|문제유사ID640건+공통1건|
|초기8파트내부차단·diag client|8673|문제유사ID8672건+공통1건|
|mode2-4 영상조건·diag client|19361|문제유사ID19360건+공통1건|

조건/길이가다르므로총건수비율을성능/개선률로해석하지않는다. 초기유사ID의fields는531095/784359/1785869로현재531399/784359/1851405와동일word가아니다. 프로파일·초기화·버전등의차이를배제하지않는다.

이번server8타입은자체networkID low25~32/high0,Parent NULL8/8,pivot-1 8/8,positionOnlyfalse8/8이었다. 클라이언트등록8건도실제Parent NULL이면서미해결부모연결ID dfbf3c0de39c400d를등록했다. actualParentType도모두null이었다. child고유ID를hook에서읽지않아server25~32와일대일매핑했다고하지않는다. hook읽기오류0,자동detach/cleanup확인,8파트physics관찰. CPU포즈존재는실제시각부드러움이아니다.

p50/p95/p99·Present·CPUbusy·GPU·malloc량·GC·경고경로self-time은미측정이다.20msp95는목표다. 게임은대기fixture에서퇴역해종료했으므로추적/등반/낙하8→64회귀가아니다. clientexit-1·serverownedcleanup,normalQuitVerifiedfalse.

## 4. 원인 분석

확인된범위:대기loop는부모·자식연결의공식native호출계약과일치한다. 서버생성시부모없는8파트,클라이언트등록시현재부모없음,하지만미해결부모요청이등록된상태를함께관찰했다. 실제의도된부모에NPC가붙어있다고설명할수없다.

지지되지않는설명:8파트로늘리면서처음생긴문제라는설명은초기단일파트증거와양립하지않는다. 진단exe만의문제라는설명은일반releaseclient의유사경고와양립하지않는다. 다만정확한bytes/조건동등성이없어모든빌드의동일버그로일반화하지않는다. 기존오프라인65~70ms급CPU비용을이네트워크경고로설명하지않는다.

미확인:서버wire에정확히무엇이기록됐는지,client no-parent flag/비트정렬/초기값중어디서참조가생기는지다. 현재데이터로잘못된서버송신·엔진버그·asset오염중하나를확정하지않는다. SETUP추가실패를지우지않고,무근거INITAI추가·부모0강제·복제/물리해제는하지않는다. 비용상중요도는별도측정이필요하다.

## 5. 검증 수준

기존경고분포감사·공식계층바인딩대조·server/client현재부모대조:제한진단성공. root cause최종확정·수정·8/64회귀·실제시각품질·성능·반복최종수용은미완료다. 전체골active.

## 6. 변경 사항

새PBO없음. 기존10모듈hash/서명검증후실행했다. 기존source/PBO/asset/physics/정상낙하복구/Steam/운영서버/개인키보존. 변경은기존research내서버mission읽기전용감사·native읽기hook·분리profile·static분석·보고서다. 서명된후보를현재배포로승격하지않았다.

## 7. 원시 증거

network-parent-contract2의plan·source·server-parent-contract.tsv·부모읽기CLI Evidence·launch/exit/TSV/RPT,network-warning-existing-controls.json와network-warning-early-role-controls.json,공식SDK enentity.c,각binding/handler/leafJSON과source를해시에연결한다. 원본기록은덮어쓰지않는다. 공개는집계/상대RVA/흐름설명이며개인키·로컬경로·프로세스·원시전체·게임자산은제외한다. pendingMCP는불변증거에포함하지않는다.

## 8. 잔여 문제

다음은입력reader의no-parent 분기와serverwriter의대응계약을연결해수정가능성을판정하는것이다. parent가NULL임을이유로nativeID를강제로0으로만들면실제이동발판/부모연결까지숨길위험이있으므로적용하지않는다. 물리query비용절감연구와network입력오류를구분하며,원인수정후에도8→64정상밀침·낙하·착지·FOLLOW·재등반과프레임/화면을독립검증한다.

로컬checkpointSHA256: fe64fdde77c11599943a6bb70aec13230f85fc969f3723df176856e1119eba99
