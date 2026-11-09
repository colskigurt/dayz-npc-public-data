# 접촉 이벤트 양성 대조와 선택적 충돌 후보 사전 검토

## 1. 작업 목표
이전8개체 관측의 EOnContact 전부0이 등록 누락인지, 관측 경로 한계인지 확인한다. 이벤트0을 네이티브 접촉 비용0으로 간주하거나 곧바로 물리 차단을 적용하지 않는다.

## 2. 실제 수행
현행 설치 scripts.pbo를 읽었다. DayZAnimal 생성자의 SetEventMask(EntityEvent.CONTACT)와 GetEventMask/Physics.IsKinematic/dBodyIsDynamic 및 dBodyCollisionBlock/dBodyRemoveBlock 선언을 확인했다. REA open_binary는PBO를unsupported_target로거절했고재시도/설치/전체Ghidra분석없이기존읽기전용PBO소스리더를재사용했다.
기존Animal 관측 코드 바이트는 유지하고 드럼통 CONTACT 관측만 추가한 격리 PBO를 빌드·서명·검증했다. NPC 이벤트 마스크·layer·simulation·collision block은 변경하지 않았다. 컴파일 전용 실행 후 headless의 원래part0인스턴스2개·드럼통2개로12게임초 관측했다. 첫 실행은 드럼통이dynamic/solid/active가 아니어서 충격을 주지 못했다. 준비실패로 보존했다.
설치 Object.CreateDynamicPhysics 선언을 확인한 뒤 두 드럼통에만 호출하는 미션1행을 추가한runtime2로수정했다. 같은진단PBO재사용·클라이언트0이었다. 두 실행을 독립적인 성공 반복으로 세지 않는다.

## 3. 측정 결과
설치 DayZAnimal 생성자 CONTACT등록은692행, EOnContact 선언은942행이다. 실제NPC eventMask16432중CONTACT비트16384는두실행모두존재했다. NPC body/dynamic/kinematic/solid/active가모두true인표본을기록했다. dynamic과kinematic이함께true라는실제반환값을보존하며임의로static이라고해석하지않는다.
첫실행:드럼통dynamic=false/solid=false/active=false·충격미적용·이벤트0. NPC AnimUpdates최대각89. 양성대조실패.
수정실행:드럼통dynamic=true/kinematic=false/solid=true,충격적용. 드럼통0/1의CONTACT총계최대515/1023,둘모두animalContact0. NPC0/1은CONTACT0,AnimUpdates/PhysicsUpdates최대각59였다. 드럼통-월드양성대조는성공했지만드럼통-NPC 또는NPC-NPC의실제접촉은확인되지않았다. 따라서NPC경로의이벤트전달자체가안된다고확정하지않는다.
모든관측서버detached=true 후RequestExit0/actual−1·강제종료없음,프로세스소멸확인. CPU/Present p50/p95/p99·native manifold·8/64기능·실제시각은미측정이다.

## 4. 원인 분석
반증:기존NPC관측0을CONTACT등록누락만으로설명하는가설. 설치소스와실제마스크모두등록을보인다.
확인:수정한일반동적물체에서는동일서버의CONTACT관측이작동했다. NPC는이격리headless조건에서kinematic으로보고됐고이벤트는없었다.
미확정:NPC들끼리실제충돌후보/접촉이있었는지,kinematic/controller경로의보고정책,그비용. 드럼통궤적은지형에따라변했으며실제NPC접촉을증명하지못했다.0이벤트=0CPU나충돌차단의효과로보고하지않는다. 격리part0두개는8/64네트워크전체구성과다르다.

## 5. 검증 수준
설치소스/실제CONTACT마스크진단성공,드럼통-월드양성대조1회성공,첫준비실패보존. NPC-NPC접촉계측은미보정·미확정. 성능개선·반복검증·기능회귀·시각·최종수용아니다. 선택적pair-block은제안만작성했고실행하지않았다.

## 6. 변경 사항
격리 nf_peer_contact_calibration PBO SHA256 4EBA8BB5CEE70FD4CF13A21DB162AEF27F420DF70458EDAD7E8E4C78661D0515,DSSignFile/DSCheckSignatures성공·개인키미포함. 원래관측PBO·게임코드·설치scripts.pbo·그래프·모델·운영서버불변. 추가setter는대조드럼통의CONTACT등록/동적물리/1회충격에한정한다. NPC는기존단일클립정밀도0설정만재사용했고추가CONTACT/layer/block변경은없다. 자료를기존폴더하위에보존했다.

## 7. 원시 증거
ANIM-peer-contact-calibration-compile1/runtime1/runtime2. installed-event-contract.json,fixture-build/signature/source,trial-contract1,mission2-change-proof,양쪽peer-calibration.tsv/plan/script/terminal,analysis2.json,selective-pair-proposal1.json을해시연결한다. 원본PBO는복사하지않고해시와선택소스행만기록했다. REA의형식거절은대상PBO형식의제한이며엔진전체분석불가능판정이아니다. 저장전해시·JSON·시험프로세스부재검증. 공개에는로컬경로·PID·키·게임자산/전체소스미포함.

## 8. 잔여 문제와 구현 전 단일 후보 제안
이벤트관측만다시반복하지않는다. 수정가능한격리후보는서로다른논리NPC의render파트중적어도하나가nonleader인쌍만dBodyCollisionBlock하는것이다. leader-leader/피해수신체/플레이어/월드는제외하며스크립트힘·모든Ray/Sphere·발지지·질량·등반·낙하·착지·FOLLOW·재등반검사는그대로다. LOD/solid/layer/업데이트빈도를바꾸지않는다.
근거:동일논리객체의중복render파트차단은이미있고,교차논리의중복기하는아직분리비용시험하지않았다. 콜백은CPU를판정하지못하므로실제조건부전체비용비교가이후진행여부를바꿀수있다. 이새후보의절감효과는미입증이며0이벤트를긍정근거로쓰지않는다.
위험/비용:8명672개·64명48384개의추가핸들,생성/조회/메모리/해제오버헤드·사망/파트재생성·네이티브위치/복제변화.1000명에서는11988000개로같은all-pairs구조가부적합하다.64용진단을대량호드설계로승격하지않는다.
다음단일산출물은8명기본OFF격리소스와핸들소유권/실패rollback/삭제전명시해제계약이다. 양쪽리더·수신체를실제ID로제외하고모든생성핸들을살아있는owner와기록해야한다. 부분실패시이미만든핸들을복구하며퇴역전해제한다. 먼저컴파일·setup/cleanup,그뒤8 OFF/ON상태계약과정상밀침/낙하/복귀를비교한다. 후보특유의실패면64확대하지않는다. baseline공통불완료는별도결함으로두고기능개발로전환하지않는다.8안전근거가있을때만64동일조건CPU/Present screen을검토한다. 단일유리결과로채택하지않고3쌍·CPU최소5%·Present악화5%이내·작업진행차이5%이내및기능/시각을별도로판정한다. 실제구현/새클라이언트는이번묶음에서시작하지않았다.
