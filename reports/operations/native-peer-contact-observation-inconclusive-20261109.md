# 네이티브 호스트 접촉 관측 — 이벤트0·비용 판별 미확정

## 1. 작업 목표
64개체 CPU 최적화 후보로 서로 다른 NPC의 추가 표시 호스트 사이 네이티브 접촉 중복이 실제 관찰되는지 확인한다. 기존 동일 NPC 내부 dBodyCollisionBlock, 스크립트 Crowd 밀침 및 지지/월드 질의와 구분한다. 비용 근거 없이 네이티브 접촉을 차단하지 않는다.

## 2. 실제 수행
기존 NFNetworkBody, 물리 readback, solid=false/NOCOLLISION 실패, installed SDK의 dBodyCollisionBlock 및 DayZAnimal 접촉 이벤트 등록 경로를 대조했다. 개체 간 block은 조사한 현행 초기화에 없고 동일 개체 내부 block만 있다. 추가 호스트 간 pair block은 아직 구현하지 않았다.
default-OFF 관측 PBO는 기존 EOnContact를 super에 위임한 뒤 카운터만 누적한다. 이벤트 마스크·충돌 레이어·solid·동적 상태·밀침·질의 주기는 바꾸지 않는다. 기존8개체 혼합 씬을 재사용해 모듈/미션 컴파일-only 서버를 먼저 확인했고, 이후60초 실제 서버/Steam retail 클라이언트 관측을 수행했다. 별도 영상·ETW·전체 덤프·성능 캡처는 없다.

## 3. 측정 결과
40개 표시 호스트의9,280행에서 enabled=true, 마지막 시각60.0153게임초. total/peer/peerRenderExtra/peerCore/same/player/world/unowned 이벤트 카운터 모두0이었다. 피해 수신체8개 존재는 mixed-hosts로 관찰했지만 수신체 이벤트 카운터는 이번 TSV에 내보내지 않았다.
8/8 목표 인식·2m이상 이동. 마지막 body solve 누적 합13,472회. 최종 outcome은 VALID_TERMINAL4, INCOMPLETE_MILESTONES3, UNRESOLVED_STATE1. 따라서8개체 전체 기능 회귀 통과가 아니다. detached=true이며 퇴역 후 종료했다. 실제64·화면 품질·CPU/Present p50/p95/p99는 미측정이다.

## 4. 원인 분석
확인: 관측 활성 필드는 실제 출력됐지만 EOnContact 양성 전달은 한 건도 확인하지 못했다. 기존 script 솔버는 계속 실행됐다.
모름: 이벤트 미전달 이유, 네이티브 접촉 매니폴드/검사 횟수·비용. 월드/플레이어 대조도0이므로 이 방법의 양성 보정이 없다. '네이티브 접촉0/비용0' 또는 '중복 접촉이 주요 병목' 어느 쪽도 입증하지 않는다. 13,472 script solve와0 callbacks는 서로 다른 층의 관측이다.
후보 위험:5표시 파트에서 대표-대표1쌍만 남기는 고정 all-pairs 방식은8개체672개,64개체48,384개의 추가 block handle을 요구한다. 이는 이론적 설정 수이지 실제 접촉 수·성능 개선률이 아니다. 수명 정리·클라이언트 복제·설정 비용·큰 무리 확장성이 미확인이다. 기존 solid=false 실패와 다르지만 새 후보의 성공 근거는 아니다.

## 5. 검증 수준
빌드/서명·실제 Enforce 컴파일 및 관측 활성/실행/종료 진단 성공. 네이티브 접촉 비용 판별은 TESTED_INCONCLUSIVE. 최적화 후보 미구현·미채택. 전체8/64 기능·시각·성능 및20ms p95 최종 수용 미완료.

## 6. 변경 사항
격리 nf_peer_contact_observer PBO SHA256 FCB11BA566CF7516655561060708393A8D24DAD9206FCFC1D0E18724E4BCFC9F. DSSignFile·DSCheckSignatures 성공, 기존 키 읽기만 사용했고 키는 생성·복사·배포 포함하지 않았다. 기존 게임/PBO/모델/애니메이션 불변. 관측 스크립트와 기존8미션의 기록 부분, 새 시험 프로필만 생성했다.
첫 클라이언트 실행은 내가 프로필 생성 시 기존 native-traversal-manifest.json을 누락해 gameplay_spawn 이전 필수 입력 검사에서 실패했다. 실패 원시 자료를 보존하고 소유 프로세스의 ID/시작 시각을 확인해 클라이언트 정상 닫기·서버 강제 정리를 했다. 이를 정상 종료/후보 기능 실패/성능 결과로 분류하지 않는다. 수정 재실행은 원래 입력 SHA256 5d72e80bb527830c337cf8e98b171538424ad06c6e410d20fb514f060f6d2fdc를 대조하고 실행 전 검증을 추가했다. 관측/게임플레이 코드는 그 재실행 사이에 변경하지 않았다.

## 7. 원시 증거
ANIM-peer-contact-observer-compile1, ANIM-peer-contact-observer-eight1(준비 실패), ANIM-peer-contact-observer-eight1-retry1. source-proof/build/signature,각 plan/launch/terminal/cleanup, native-peer-events.tsv/gameplay-eight.tsv/mixed-hosts.tsv/terminal-state.tsv와 스크립트 로그 보존. 재실행 서버 exit-1·forced=false,클라이언트 CloseMainWindow 후 종료·forced=false. 종료코드를0으로 바꾸지 않는다. 저장 전 관련 프로세스 부재·원본 및 결과 파일 무결성을 확인한다. 공개에는 집계/상대 증거 ID만 제공하며 로컬 경로·프로세스·키·전체 자산은 제외한다.

## 8. 잔여 문제
실제 네이티브 접촉 비용은 이번 callback 방식으로 판별하지 못했다. 따라서 추정에 근거한 추가 표시 호스트 간 충돌 차단은 구현하지 않는다. 반복0을 얻기 위한 같은 관측 재실행이나64 확대도 하지 않는다. CPU 주력 목표는 미해결이며 기존 Ray/Sphere 질의 비용 근거를 유지한다. 새 물리 절감 후보를 선정할 때는 기능 허용 오차와 호출량/실제 전체 비용을 구분하고, 관측 경로가 의사결정에 필요한 증거를 제공할 수 있는지 먼저 확인해야 한다. 이번 부분 기능 결과를 기능 개선 성과로 보고하지 않는다.
