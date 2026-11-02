## 1. 작업 목표

미해결ID를 반복조회하는 원인을 레코드 최초등록까지 추적한다. 이전 full entity setup OFF/ON 후보가 경고를없애지못한 결과를 보존한다. 새로운 충돌/지지/낙하/시계 수정은 하지 않는다.

## 2. 실제 수행

기존 직접caller검색기를 재사용해대기등록a09cc0→a03460→a1a990의 상위caller를원본PE/Capstone으로읽었다. 동일 해시A500 진단클라이언트의1NPC/8파트기존fixture에서 생성setup후보OFF·동일10PBO를 사용하고, 클라이언트연결전에a03460입구를원본10bytes가드뒤읽기전용Frida계측했다. 최대8입력후listener자동detach,55초window·60초CLI후cleanup을확인했다. 게임필드/ID/시계쓰기없음. 새영상/덤프없음.

기존2시간deadline Ghidra요청은 강제중단없이 약17분후 정상완료했다. CLIexit0,oktrue,subjectA500hash와출력SHA를검증했다. 초기15분timeout실패는보존한다. 완료의사코드와기존어셈블리의 null→유지→재삽입 흐름을대조했다. 별도 후속MCP의0x140c044f8분석은 아직응답대기이며이번완료증거에포함하지않는다.

## 3. 측정 결과

최초등록입력8건:모두동일ID dfbf3c0de39c400d,receiver NetworkClient·child타입DayZAnimal,caller a1a9d9→b59749→c044f8. 읽기오류0,메시지truncationfalse,listenerdetach확인,cleanup_error null. child주소/고유ID를기록하지않아8건을서로다른8파트의일대일매핑으로주장하지않는다. RTTI는이번에bounded byte열의첫NUL까지읽어이전인접메타데이터혼입을피했다.

클라이언트script감사에서8타입·모든physics true·머리CPU포즈표본이있었다. 이것은화면부드러움/FPS측정이아니다. 미션은대기fixture에서퇴역시켜종료했으므로8/64추적·등반·전투회귀가아니다. clientexit-1·serverownedcleanup,normalQuitVerifiedfalse. p50/p95/p99·Present·CPUbusy·GPU·질의별self-time·malloc량·GC·시각개선은미측정이다.20msp95는목표다.

Ghidra시간1029.653초는분석도구의총경과시간이며NPC실행비용이아니다. 8등록표본은독립반복8회가아니다.

## 4. 원인 분석

확인:조회중나중에변한값만본것이아니라등록시점부터동일한미해결참조가들어왔다. c044f8이포함된원본fragment는바이트버퍼·비트커서에서값을읽어b59610으로전달한다. 따라서상위경로는스크립트API호출이라고확정할수없으며현재증거는native네트워크입력처리와양립한다. 이fragment의전체논리함수경계와필드이름은미확정이다.

완료의사코드확인:a12460은대기항목을24byte임시항목으로복사하고원래목록을비운다. lookup이null이면해당임시항목을유지한다. 남은항목은a09cc0으로다시등록하고임시배열을해제한다. 이전‘항목을남긴다’는설명을copy/clear/reinsert구조로정밀화한다. decompiler표현은원본소스가아니며allocatorhelper횟수/바이트/self-time의실측으로바꾸지않는다.

아직미확인:서버가잘못된값을보냈는지,클라이언트해석중엇갈렸는지,부모참조/계층연결의정확한필드계약인지,최종전체비용기여다. 상수ID반복만으로원본모델오염·메모리훼손·전체프로토콜불일치를확정하지않는다. unsignedID를월드좌표로해석하지않는다. 단순레이캐스트축소만으로이경로를해결할수있다고하지않는다.

## 5. 검증 수준

최초등록입력·실제caller·정확한RTTI접두부:제한진단성공. 기존대기loop의정적의사코드확보:진단성공. 원인최종확정·코드수정·8/64회귀·시각품질·성능·최종수용:미완료. 전체골active. 기존기능과사용자의애니메이션개선관찰을부정하거나완료로확대하지않는다.

## 6. 변경 사항

새PBO없음. 기존10PBOhash/서명검증후실행,SETUP후보는OFF. 서명된765E네트워크factory·8파트asset·접촉/복구솔버를보존했다. 변경은기존research내의작은등록관측스크립트·독립profile/mission·원본PE의bounded분석·완료Evidence와보고서다. 게임실행파일·운영서버·Steam·원본ANM/XOB/그래프·이전증거·개인키는변경하지않았다. 현재기존PBO의서명검증성공이며새서명완료를주장하지않는다.

## 7. 원시 증거

sourceRunId network-gameplay/network-registration-origin1. network-registration-probe1의plan/byteguard/CLI Evidence/exit/launch/role/TSV/RPT, network-record-producer/registration/dispatch callers, script-stackfragmentJSON, 완료network-resolver-ghidraJSON/state/런처를해시검증한다. 공개는집계·상대RVA·흐름설명만제공하고원시전체/게임자산/프로세스/로컬경로/키를제외한다. 후속MCPpending출력은불변증거에포함하지않는다.

## 8. 잔여 문제

다음단일산출물은네트워크참조writer/reader계약차이의판정이다. 기존바닐라자료에서동일현상유무를먼저확인하고, 필요시native직렬화의대응필드와실제부모/ID를읽기전용으로연결한다. 실패한SETUP변경을8→64로확대하지않는다. 구체적인수정가능원인이생기기전엔부모를강제로0으로설정하거나replication/physics를끄지않는다. 비용감소후보와시각품질은여전히별도판정한다.

로컬checkpointSHA256: 01c606237c33cadebe836489375de7580110563d1e23256d282b40c4940bbe96
