# client mode2 정적 계약: 별도 buffer 시계와 공유 pose 소비 경로

작성: 2026-11-01T15:54:59.379525+09:00

## 1. 작업 목표

서버 분기에서 돌아와 기존 오프라인 mode0 시간 손실과 실제 네트워크 클라이언트 mode2의 실행 계약 차이를 확인한다. 역할 변경이 정당한 후속 비교인지 판단하고, 동일한 초기조건이 아닌 시험을 개선 결과로 오인하지 않도록 한다.

## 2. 실제 수행

해시가 고정된 client c6709ece의 scheduler와 mode2 분기를 읽었다. 기존 함수의 첫 unwind fragment만 보고 끝내지 않고 명시적 범위 안에서 직접 분기를 순회했다. scheduler, buffer dispatcher, 후속 상태 처리와 두 flag getter의5개 함수에 더해 버퍼의3개 소비 함수를 확인했다. 각 명령의 원본 바이트 및 RTTI subobject slot4개를 다시 대조했다.

실제 파일의 프로토타입 Animal_DGNativeProbe/DGNativeCommand 소스에서 명령 시작·요청 상태 전달의 문맥10개를 확인했다. 전체 상속 계층이나 native 전송을 검색 부재만으로 배제하지 않았다. 게임·클라이언트·서버·워크벤치를 새로 실행하지 않았다.

## 3. 측정 결과

정적 주 함수5개, 추가 소비 함수3개, 실제 바이트 재대조 명령 레코드 531개, subobject slot4개를 확보했다. 명시한 창 안의 직접 branch 외부 이탈0이다. 외부 call과 가상 call까지 모두 분석됐다는 뜻은 아니다. 신규 runtime0·live mode2 표본0·새 PBO0·영상0이다.

mode2는 entity+0x5f8을 buffer dispatcher0x140997b70에 넘긴다. 이 함수는 subobject+0x18을 입력 시간으로 증가시키고 slot+0x30에서 받은 간격과 비교하며, 버퍼 항목 수+0x14를 사용해 항목을 처리하고 간격을 빼는 루프를 수행한다. mode0의 entity+0x66c 누적 후0으로 초기화하는 경로와 같은 누적기가 아니다.

Animal의 해당 subobject vtable은0x14130fef8이며 offset0x5f8이다. slot+0x28은 entity base를 반환하고, slot+0x30은 interval 선택0x140b70100, slot+0x48은 descriptor 소비0x140b70490에 연결된다. slot+0x20은 버퍼 항목 처리0x140b71af0에 연결된다.

slot+0x48 소비자는 buffer+0x38이 양수일 때 descriptor를 만들고 mode0과 같은 하류 보간 함수0x140b13380을 호출한다. 이어 pose 적용 함수0x1404347c0/0x1404356b0으로 연결된다. 정적으로 별도 buffered 경로가 존재함은 확인했지만 실제 수신 데이터·호출 빈도·화면 결과는 이번에 측정하지 않았다.

이번 자료에는 frame-time p50/p95/p99,8/64 안정성,화면·GPU·할당/GC의 새로운 수치가 없다.20ms p95는 목표다.

## 4. 원인 분석

확인: 오프라인 mode0과 mode2를 같은 dt 누적기로 설명하면 잘못이다. mode2에는 별도 subobject 시계·항목 버퍼·descriptor 전달이 있고, 최종 보간 소비 함수 일부만 공유한다. 따라서 offline mode0의 잔여시간 수정 후보를 mode2에 그대로 적용하지 않는다.

가설: 실제 server/client 역할 비교는 지원되는 다른 경로를 평가할 연구 가치가 있다. 다만 원격 개체가 생성돼 mode2를 사용하는지, 버퍼에 자료가 들어오는지, custom command와 요청 상태가 같은지부터 확인해야 한다. 서버의 PreviewRequest 필드가 client에 똑같이 적용될 것이라는 가정은 현재 근거로 검증되지 않았다. 상속된 native 전송이 없다는 뜻도 아니다.

반증 범위: 이번 정적 결과는 mode2도 mode0과 똑같이 entity+0x66c를0으로 만드는 경로라는 가정과 배치된다. 반면 네트워크로 실행하면 끊김이 해결된다는 주장은 입증하지 않았다. 기존 mode0의 실제 CPU/native/화면 시간 손실 근거는 보존한다.

## 5. 검증 수준

mode2의 별도 buffer 계약과 공유 하류 소비자 연결은 제한적 정적 진단 성공이다. 실제 네트워크 역할 비교,동작 동등성,기능 회귀,시각 품질,성능 개선,반복 검증 및 최종 수용은 미실행이다.

## 6. 변경 사항

작은 PE 분석기·파생 JSON·보고서만 추가했다. 게임 코드·PBO·ANM·기존 시험 하네스·운영 서버·9350 후보·개인키는 변경하지 않았다. 새 PBO가 없어 서명 대상이 없으며, 이번에 기존 PBO 서명을 새로 검증한 것으로도 기록하지 않는다. 도구 설치와 대용량 로그·영상은 없다.

## 7. 원시 증거

실행/분석 ID animation-client-mode2-20261101-1555. inspect_mode2.py,mode2-contract.json,policy-types.json,mode2-subobject-slots.json,mode2-buffer-consumers.json을 연결한다. immutable client 해시·함수 주소·명령 바이트·원본 소스 행번호와 해시를 보존한다. 이 결과는 기존 Capstone 기반 정적 분석이며 REA runtime Evidence가 아니다. 공개본은 선정 호출 문맥과 파생 명세이며 전체 게임 실행 파일·자산·로컬 경로·키는 제공하지 않는다.

## 8. 잔여 문제

다음은1개 물리 native part와 변경 없는 PBO로 격리 dedicated server와 연결 client를 사용하는 최소 역할 시험이다. 우선 stream-in,실제 mode2,buffer 존재,graph/pose 진행,요청 상태의 동등성을 확인한다. 하나라도 성립하지 않으면 모드 값을 직접 쓰거나 전체 NPC 권한 구조를 옮기지 않고 그 실패를 분리한다. 실제 동등성이 성립한 뒤에만 시간·화면 품질 비교를 진행한다.

필요 자원은 기존 server/client와 bounded 관찰기다. 클라이언트 실행 전 미리 알리고, 새 비교에 필요한 조건을 고정한다. 새 보간·속도 보정·매 틱LOD 강제 쓰기는 보류한다. 해시 불일치·crash·관찰 범위 이탈은 중단 조건이다.64개체 기능·비용은 이 역할 가능성 진단과 별도 실험으로 유지한다.

선정 공개 본문. 로컬 보고서 SHA-256: `05aa769229a6516ecfcab8f52349f82d239a62a43aa96282f09ae7967a6f71a4`. 원시 전체가 아니다.
